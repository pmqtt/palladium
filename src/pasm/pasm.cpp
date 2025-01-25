#include "VirtualMachine.h"
#include "Util.h"
#include <filesystem>
#include <format>
#include <ostream>
#include <fstream>
#include <vector>

template <typename... Args> void print(std::format_string<Args...> fmt, Args&&... args) {
  std::print(std::cout, fmt, std::forward<Args>(args)...);
}

struct PasmOption {
  std::string output_file;
};

void fatal() {
  print("pasm: fatal: no input file specified\n Type pasm -h for help.\n\n");
}

void usage() {
  print("Usage: pasm [options...] filename\n");
  print("       pasm -v\n");
  print("Options (values in brackets indicate defaults):\n\n");
  print("    -h            show this text and exit\n");
  print("    -v            print the pasm version number and exit\n");
  print("    -o outfile    write output to outfile\n");
}

void version() {
  print("pasm version 0.0.1 compiled on ...\n");
}

void print_usage_and_exit_if(bool condition) {
  if (condition) {
    usage();
    std::exit(0);
  }
}

auto options(const std::vector<std::string>& args) -> PasmOption {
  PasmOption opt;
  for (std::size_t i = 1; i < args.size(); ++i) {
    if (args[i] == "-v") {
      version();
      std::exit(0);
    }
    if (args[i] == "-h") {
      print_usage_and_exit_if(true);
    }
    if (args[i] == "-o") {
      print_usage_and_exit_if(i + 1 >= args.size());
      opt.output_file = args[i + 1];
      i++;
    }
  }
  return opt;
}

void inc_index_if_whitespace(std::size_t& i, const std::string& str) {
  if (i >= str.length()) {
    return;
  }
  if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
    for (; i < str.length(); ++i) {
      if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
        break;
      }
    }
  }
}

void store_value_until_whitespace(std::size_t& i, std::string& value, const std::string& str) {
  for (; i < str.length(); ++i) {
    if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
      break;
    }
    value += str[i];
  }
}

void extract_value(std::string& value, const std::string& line) {
  std::size_t i = 0;
  inc_index_if_whitespace(i, line);
  store_value_until_whitespace(i, value, line);
}

void parse_function_entry_item(std::size_t& i, std::string& value, const std::string& entry, std::size_t line_nr) {
  inc_index_if_whitespace(i, entry);
  store_value_until_whitespace(i, value, entry);
  if (value.length() == 0) {
    std::string trimmed_entry = trim_copy(entry);
    print("pasm: Missing function entry item in line {}! Content is {}\n", line_nr, trimmed_entry);
    exit(1);
  }
}

auto make_function_entry(const std::string& entry, std::size_t line_nr) -> FunctionEntry {
  std::string fname;
  std::string argC;
  std::string label;
  std::size_t i = 0;

  parse_function_entry_item(i, fname, entry, line_nr);
  parse_function_entry_item(i, argC, entry, line_nr);
  parse_function_entry_item(i, label, entry, line_nr);
  inc_index_if_whitespace(i, entry);
  if (i < entry.length()) {
    print("pasm: parsing error in function section line {}! valid format is: [function name] [argument] [count label] "
          "not {}\n",
          line_nr, entry);
    std::exit(1);
  }
  bool is_zero = false;
  if (argC.length() == 1 && argC[0] == '0') {
    is_zero = true;
  }
  uint8_t arg_count = std::atoi(argC.c_str());

  if (arg_count == 0 && !is_zero) {
    print("pasm: parsing error in function section in line {}! argument count is not a number: {}\n", line_nr, argC);
    std::exit(1);
  }

  return FunctionEntry(fname, arg_count, label);
}

auto parse_function_entries(std::ifstream& stream, std::size_t& line_nr) -> std::vector<FunctionEntry> {
  std::string entry;
  std::vector<FunctionEntry> f_entries;
  bool valid_block = false;
  while (!std::getline(stream, entry).eof()) {
    line_nr++;
    std::string first_item;
    extract_value(first_item, entry);
    if (first_item.length() > 0 && first_item[0] == '#') {
      // comment
      continue;
    }
    if (first_item == "$end") {
      valid_block = true;
      break;
    }
    FunctionEntry e = make_function_entry(entry, line_nr);
    f_entries.push_back(e);
  }
  if (!valid_block) {
    print("pasm: parsing error in function section! function section not closed with $end\n");
    std::exit(1);
  }
  return f_entries;
}

void check_is_stream_open(std::ifstream& stream, const std::string& file) {
  if (!stream.is_open()) {
    if (!std::filesystem::is_regular_file(file)) {
      print("pasm: fatal: unable to open file `{}' File path doesn't refer to a regular file\n", file);
    } else {
      print("pasm: fatal: unable to open file `{}' Possible cause:\n", file);
      print("       - No read access to the file.\n");
      print("       - The file is locked by  another process.\n");
      print("       - A file system error occured.\n");
      print("       - Unknown error.\n");
    }
    std::exit(1);
  }
}

void run(const PasmOption& opt, const std::string& file) {
  std::ifstream stream(file, std::ios::binary);
  check_is_stream_open(stream, file);
  std::string line;
  std::size_t line_nr = 0;
  std::vector<FunctionEntry> function_entries;
  while (!std::getline(stream, line).eof()) {
    line_nr++;
    std::string header;
    extract_value(header, line);
    if (header.length() > 0 && header[0] == '#') {
      continue;
    }
    if (header == "$function_section") {
      if (function_entries.size() == 0) {
        function_entries = parse_function_entries(stream, line_nr);
      } else {
        print("pasm: fatal: multiple function_section found in line {}!", line_nr);
        std::exit(0);
      }
    }
    if (header == "$code_section") {
      // parse_code_section(stream, line_nr);
    }
  }
}

auto main(int argc, char** argv) -> int {
  if (argc < 2) {
    fatal();
    return 1;
  }
  std::vector<std::string> args(argv, argv + argc);
  PasmOption opt;
  if (argv[1][0] == '-') {
    opt = options(args);
  }
  std::string input_file = *(args.end() - 1);
  if (!std::filesystem::exists(input_file)) {
    print("pasm: fatal: unable to open input file `{}' No such file or directory\n", input_file);
    std::exit(1);
  }

  run(opt, input_file);
  return 0;
}
