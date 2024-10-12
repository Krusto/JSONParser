#include <locale.h>

#define CUTILS_VERBOSE
#include "JSONParser.h"

int main(void)
{
    setlocale(LC_ALL, "");

    JSONParserT parser = {NULL};

    JSONParserResultT parseResult = json_parse_file("./assets/test.json", &parser);

    // if (JSON_PARSE_RESULT_OK != parseResult) { return -1; }

    printf("\n\n\n\n\n\n\n");
    print_node(parser.root, 0);
    destroy_json_parser(&parser);

    return 0;
}