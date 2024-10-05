#include <locale.h>

#define CUTILS_VERBOSE
#include "JSONParser.h"

int main(void)
{
    setlocale(LC_ALL, "");
    JSONParserT parser;

    JSONParserResultT parseResult = json_parse("assets/test.json", &parser);

    // if (JSON_PARSE_RESULT_OK != parseResult) { return -1; }

    printf("\n\n\n\n\n\n\n");
    // print_node(parser.root);
    free_json_tree(parser.root);
    return 0;
}