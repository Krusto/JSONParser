#include <locale.h>

#define CUTILS_VERBOSE
#include "JSONParser.h"

int main(void)
{
    setlocale(LC_ALL, "");

    JSONParserT parser = {NULL};
    parser.verboseOutput = 1;
    JSONParserResultT parseResult = json_parse_file("./assets/test.json", &parser);

    if (JSON_PARSE_RESULT_OK != parseResult)
    {
        LOG_ERROR("Error while parsing JSON file!\n");
        return -1;
    }

    json_print_tree(parser.root, 0);
    destroy_json_parser(&parser);

    return 0;
}