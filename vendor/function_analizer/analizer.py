import re
import sys


def replace_in_tuple_list(value, new_value, tuple_list):
    result = []
    for index in range(len(tuple_list)):
        line = tuple_list[index][1].replace(value, new_value).strip()
        result.append([tuple_list[index][0], line])
    return result


def find_in_tuple_list(value, tuple_list):
    Output = [
        item for item in tuple_list if item[1].find(value) != -1 or item[0] == value
    ]
    return Output[0]


def find_in_tuple_list_inline(value, tuple_list):
    Output = [
        item for item in tuple_list if item[1].find(value) != -1 or item[0] == value
    ]
    if len(Output) > 0:
        return Output[0]
    return []


def get_code(file_path):
    try:
        with open(file_path, "r") as file:
            code = file.read()
            return code
    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    return


def get_lines(file_path):
    try:
        with open(file_path, "r") as file:
            file_lines = file.readlines()
            return file_lines
    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    return []


def extract_functions_from_lines(file_lines):
    static_functions = []
    inline_static_functions = []
    for line_number, line in enumerate(file_lines, start=1):
        line = line.split("{", 1)[0].strip()
        line = line.replace(";", "").strip()
        if line.startswith("static"):
            static_functions.append((line_number, line))
    for i in range(len(file_lines)):
        line_number = i
        line = file_lines[i]
        original_line = line
        line = line.split("{", 1)[0]

        if line.startswith("inline static"):
            if original_line.find("{") != -1:
                line += "{"
            if line.endswith(";"):
                print(
                    f"\nError: Invalid inline static function definition on line {line_number+1}.\n"
                )
                return []
            inline_static_functions.append((line_number + 1, line))
    return [static_functions, inline_static_functions]


def extract_functions_from_code(code, function_name):
    pattern = re.compile(rf"\b{function_name}\b\s*\([^)]*\)\s*{{", re.DOTALL)
    match = pattern.search(code)
    if not match:
        return None
    start_index = match.end()
    brace_count = 1
    i = start_index

    while brace_count > 0 and i < len(code):
        if code[i] == "{":
            brace_count += 1
        elif code[i] == "}":
            brace_count -= 1
        i += 1
    return code[start_index : i - 1]


def extract_function_names(functions):
    function_names = []
    pattern = re.compile(rf"\w+\(")
    for line, function in functions:
        match = pattern.search(function)
        if match:
            function_names.append(match.group(0).strip().replace("(", ""))
    return function_names


def extract_functions(filePath):

    file_lines = get_lines(filePath)
    code = get_code(filePath)
    inline_function_declarations = extract_functions_from_lines(file_lines)[1]
    static_function_declarations = extract_functions_from_lines(file_lines)[0]
    functions = [[], [], [], []]
    function_names = extract_function_names(inline_function_declarations)
    for i in range(len(function_names)):
        function_name = function_names[i]
        function_body = extract_functions_from_code(code, function_name)
        functions[0].append(inline_function_declarations[i][1])
        if inline_function_declarations[i][1].find("{") == -1:
            function_body = "{" + function_body
        functions[1].append(function_body + "}")
        functions[2].append(function_name)
        functions[3].append(inline_function_declarations[i][0])
    functions.append(static_function_declarations)
    functions.append(inline_function_declarations)
    return functions


def fix_function_order(file_path, definition_index=0):
    functions = extract_functions(file_path)
    declarations = functions[4]
    definitions = functions[5]
    if functions[2][definition_index].find("free_json_tree") != -1:
        print("free_json_tree")

    if len(declarations) != len(definitions):
        print("\nError: Function declarations and definitions do not match.")
        return -2

    if len(declarations) != 0:
        for i in range(definition_index, len(declarations)):
            definition = (
                definitions[i][1].replace("inline", "").replace("{", "").strip()
            )
            if declarations[i][1] != definition:
                line_coming_from = find_in_tuple_list_inline(
                    declarations[i][1], definitions
                )[0]

                print("\nError: Function declaration and definition do not match.")
                print("Implementation for:\n    {}".format(declarations[i][1]))
                print(
                    "Found on line {} should be on line {}\n".format(
                        line_coming_from, definitions[i][0]
                    )
                )
                print("Fixing it...")
                line_number_to_be_placed = definitions[i][0]
                index = 0
                for j in range(len(functions[3])):
                    if functions[3][j] == line_coming_from:
                        index = j
                        break

                function_declaration_to_replace = functions[0][index]
                function_body_to_replace = functions[1][index]
                code = get_code(file_path)
                code = code.replace(function_declaration_to_replace, "")
                code = code.replace(function_body_to_replace, "")

                with open(file_path, "w") as file:
                    file.write(code)
                lines = get_lines(file_path)
                lines[line_number_to_be_placed - 1] = (
                    function_declaration_to_replace
                    + function_body_to_replace
                    + lines[line_number_to_be_placed - 1]
                )

                with open(file_path, "w") as file:
                    file.writelines(lines)

                print("Done!")
                fix_function_order(file_path, definition_index + 1)
                return -1
    else:
        return 0


def check_static_functions(static_functions, inline_static_functions):
    removed_inline_static = replace_in_tuple_list("inline", "", inline_static_functions)

    print("Checking for missing inline static function definitions...")
    for line_number, line in static_functions:
        isFound = find_in_tuple_list(line, removed_inline_static)
        if not isFound:
            print(f"NOT FOUND: {line} (Missing inline static definition)")
        else:
            print(f"FOUND: {line} (inline static definition found)")

    print("\nChecking for missing static function declarations...")
    for line_number, line in removed_inline_static:
        isFound = find_in_tuple_list(line.replace("{", "").strip(), static_functions)
        if not isFound:
            print(f"NOT FOUND: {line} (Missing static declaration)")
        else:
            print(f"FOUND: {line} (static declaration found)")


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Usage: python analyzer.py <file_path>")
        sys.exit(1)
    path = sys.argv[1]
    # path = "include/JSONParser.h"
    print("Extracting Functions...")

    result = extract_functions(path)

    if result:
        print("Checking Static Functions...")
        check_static_functions(result[4], result[5])
        print("\n\n\n")
        print("Checking And Fixing Function Order...")
        result = fix_function_order(path)

        if result == 0:
            print("Fixed all functions successfully!")
    else:
        print("Error: Failed to extract functions.")
