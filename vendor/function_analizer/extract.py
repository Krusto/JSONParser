import re

def find_implementation(declaration,definitions):
    for i in range(len(definitions)):
        line = definitions[i][1]
        number = definitions[i][0]
        if line.startswith(declaration):
            return definitions[i][0]
    return -1

def check_function_order(declarations,definitions):
    for i in range(len(declarations)):
        if declarations[i][1] != definitions[i][1]:
            print("Error: Function declaration and definition do not match.")
            print("{} != {}".format(declarations[i][1],definitions[i][1]))
            print("implementation for:\n {} \nfound on line {} should be on line {}\n\n"
                  .format(declarations[i][1],find_implementation(declarations[i][1],definitions),definitions[i][0]-1))
def check_static_functions(file_path):
    try:
        with open(file_path, 'r') as file:
            static_lines = []  
            inline_static_lines = []  
            file_lines = file.readlines()  

            for line_number, line in enumerate(file_lines, start=1):
                line = line.split('{', 1)[0].strip()
                line = line.replace(';', '').strip()
                if line.startswith('static'):
                    static_lines.append((line_number, line))
                    
            for line_number, line in enumerate(file_lines, start=1):
                line = line.split('{', 1)[0].strip()
                line = line.replace(';', '').strip()
                if line.startswith('inline static'):
                    inline_static_lines.append((line_number, line.replace('inline', '').lstrip()))
            
            inline_static_pattern = r'^static.*'
            for line_number, line in static_lines:
                found_clone = False
                for other_line in file_lines:
                    other_line = other_line.split('{', 1)[0].replace(';', '').strip()
                    removedInline = other_line.replace('inline', '').lstrip()
                    if other_line.startswith("inline"):
                        if removedInline == line:
                            found_clone = True
                            break
                
                if found_clone:
                    print(f"{line_number}: {line} (inline static copy found)")
                else:
                    print(f"NOT FOUND {line_number}: {line} (Missing inline static clone)")
            check_function_order(static_lines,inline_static_lines)

    except FileNotFoundError:
        print(f"Error: The file '{file_path}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
check_static_functions('../../include/JSONParser.h')