#include <locale.h>

#define CUTILS_VERBOSE
#include "JSONParser.h"

int main(void)
{
    setlocale(LC_ALL, "");

    JSONParserT parser = {NULL};

    JSONParserResultT parseResult = json_parse("/home/banitsa/Projects/JSONParser/bin/assets/test.json", &parser);

    // if (JSON_PARSE_RESULT_OK != parseResult) { return -1; }

    printf("\n\n\n\n\n\n\n");
    print_node(parser.root);
    destroy_json_parser(&parser);

    return 0;
}