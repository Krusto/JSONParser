#define CUTILS_VERBOSE
#include "JSONParser.h"

int main(void)
{
    JSONParserT parser;

    JSONParserResultT parseResult = json_parse("assets/test.json", &parser);

    if (JSON_PARSE_RESULT_OK != parseResult) { return -1; }
    return 0;
}