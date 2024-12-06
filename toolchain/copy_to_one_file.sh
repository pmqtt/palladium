#/bin/bash

function reduce() {
  sed -E '
    # Remove single-line comments
    s://.*$::g;
    # Remove multi-line comments
    :start
    s:/\*[^*]*\*+([^/*][^*]*\*+)*/::g;
    /\/\*/{
        N
        b start
    }
    # Trim leading and trailing whitespace
    s/^[[:space:]]+//g;
    s/[[:space:]]+$//g;
    # Remove empty lines
    /^[[:space:]]*$/d;
    # Remove redundant line breaks (merge lines with trailing semicolons or braces)
    N
    s/\n[[:space:]]*([{};])/\1/g;
    t
    # Restore single line after cleaning
    P
    D
' "$1" >"$2"

}

echo "CMakeLists.txt" >gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt
cat CMakeLists.txt >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt

echo "src/CMakeLists.txt" >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt
cat src/CMakeLists.txt >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt

echo "tests/CMakeLists.txt" >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt
cat tests/CMakeLists.txt >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt

for f in src/*.cpp; do
  echo "$f" >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
  out_put="$f".txt
  reduce "$f" "$out_put"
  cat "$out_put" >>gpt_critic.txt
  rm "$out_put"
  echo "------------------------------" >>gpt_critic.txt
done

for f in include/*.h; do
  echo "$f" >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
  out_put="$f".txt
  reduce "$f" "$out_put"
  cat "$out_put" >>gpt_critic.txt
  rm "$out_put"
  echo "------------------------------" >>gpt_critic.txt
done

for f in tests/*.cpp; do
  echo "$f" >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
  out_put="$f".txt
  reduce "$f" "$out_put"
  cat "$out_put" >>gpt_critic.txt
  rm "$out_put"

  echo "------------------------------" >>gpt_critic.txt
done
