import subprocess
import os
import re

# Pfade anpassen
PASM_EXECUTABLE = "./pasm"  # Pfad zur ausführbaren Datei
TEST_DIR = "./tests/pasm_tests"    # Ordner mit den Testdateien

def run_test(input_file):
    # Input-Datei lesen
    with open(input_file, "r") as file:
        lines = file.readlines()

    # Erwartetes Ergebnis auslesen (z. B. # RESULT: [...])
    expected_result = None
    for line in lines:
        match = re.match(r"#[ ]*[Rr][Ee][Ss][Uu][Ll][Tt]:\s*(.*)", line)
        if match:
            expected_result = match.group(1).strip()
            break

    if expected_result is None:
        print(f"❌ Expected result not definied in {input_file}!")
        return False

    # `pasm` mit der Input-Datei ausführen
    try:
        result = subprocess.run(
            [PASM_EXECUTABLE, input_file],
            input="".join(lines),  # Input übergeben
            text=True,             # Textausgabe aktivieren
            capture_output=True    # Ausgabe abfangen
        )
    except FileNotFoundError:
        print(f"❌ Executable '{PASM_EXECUTABLE}' not found!")
        return False

    # Ausgabe überprüfen
    actual_result = result.stdout.strip()
    if len(expected_result) > 0 and actual_result == expected_result:
        print(f"✅ Test {os.path.basename(input_file)} passed!")
        return True
    else:
        if len(expected_result)==0:
            if result.returncode == 0:
                print(f"✅ Test {os.path.basename(input_file)} passed!")
                return True
        print(f"❌ Test {os.path.basename(input_file)} failed!")
        print(f"  Expected: {expected_result}")
        print(f"    Result: {actual_result}")
        return False

def main():
    # Alle `.input`-Dateien im Testverzeichnis finden
    input_files = [f for f in os.listdir(TEST_DIR) if f.endswith(".pasm")]

    if not input_files:
        print("❌ No input file found!")
        return

    # Tests ausführen
    passed = 0
    failed = 0
    for input_file in input_files:
        if run_test(os.path.join(TEST_DIR, input_file)):
            passed += 1
        else:
            failed += 1

    # Zusammenfassung
    print("\n--- Test conclusion ---")
    print(f"Passed: {passed}")
    print(f"Failed: {failed}")
    print(f"Sum: {len(input_files)}")

if __name__ == "__main__":
    main()
