#ifndef JSONPARSER_HEADER
#define JSONPARSER_HEADER
/**
 * @file
 * @author Krusto Stoyanov ( k.stoianov2@gmail.com )
 * @brief 
 * @version 1.0
 * @date 
 * 
 * @section LICENSE
 * MIT License
 * 
 * Copyright (c) 2024 Krusto
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @section DESCRIPTION
 * 
 * JSONParser Header
 */


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CFilesystem.h"
#include "CLog.h"
#include "JSONParserDefs.h"
#include "STDTypes.h"
#include <locale.h>
/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/
#define UNICODE_TABULATION 0x0009
#define UNICODE_LINE_FEED 0x000A
#define UNICODE_CARRIAGE_RETURN 0x000D
#define UNICODE_SPACE 0x0020
#define UNICODE_LEFT_SQUARE_BRACKET 0x005B
#define UNICODE_LEFT_CURLY_BRACKET 0x007B
#define UNICODE_RIGHT_SQUARE_BRACKET 0x005D
#define UNICODE_RIGHT_CURLY_BRACKET 0x007D
#define UNICODE_COLON 0x003A
#define UNICODE_COMMA 0x002C
#define UNICODE_t 0x0074
#define UNICODE_f 0x0066
#define UNICODE_n 0x006E
#define UNICODE_QUOTATION_MARK 0x0022
#define UNICODE_BACK_SLASH 0x005C

/***********************************************************************************************************************
Static Variables
***********************************************************************************************************************/

static const char* tokens_str[] = {"UNICODE_TOKEN_NONE",
                                   "UNICODE_TOKEN_LEFT_SQUARE_BRACKET",
                                   "UNICODE_TOKEN_RIGHT_SQUARE_BRACKET",
                                   "UNICODE_TOKEN_LEFT_CURLY_BRACKET",
                                   "UNICODE_TOKEN_RIGHT_CURLY_BRACKET",
                                   "UNICODE_TOKEN_COLON",
                                   "UNICODE_TOKEN_COMMA",
                                   "UNICODE_TOKEN_QUOTATION_MARK",
                                   "UNICODE_TOKEN_TRUE",
                                   "UNICODE_TOKEN_FALSE",
                                   "UNICODE_TOKEN_NULL",
                                   "UNICODE_TOKEN_BACK_SLASH"};

/***********************************************************************************************************************
Static function declarations
***********************************************************************************************************************/
NodeT* ParseString(JSONParserT* parser);
NodeT* ParseArray(JSONParserT* parser);
NodeT* ParseObject(JSONParserT* parser);
NodeT* ParseValue(JSONParserT* parser);
wchar_t current_unicode_char(JSONParserT* parser);
uint8_t current_char_length(JSONParserT* parser);
BOOL is_char_space(wchar_t current_unicode_char);
UnicodeTokenT get_token(wchar_t unicodeChar);
void move_to_next_char(JSONParserT* parser);

void PrintNode(NodeT* node)
{
    switch (node->valueType)
    {
        case NODE_TYPE_OBJECT: {
            NodeObjectT* object = ((NodeObjectT*) node);
            DArrayT* elements = object->elements;
            for (size_t i = 0; i < darr_length(elements); i++)
            {
                NodeT* element = (NodeT*) *(long long*) darr_get_ptr(elements, i);
                PrintNode(element);
            }

            break;
        }
        case NODE_TYPE_ARRAY:
            for (size_t i = 0; i < darr_length(((NodeArrayT*) node)->data); i++)
            {
                PrintNode((NodeT*) *(long long*) darr_get_ptr(((NodeArrayT*) node)->data, i));
            }
            break;
        case NODE_TYPE_OBJECT_ELEMENT: {
            NodeObjectElementT* object = ((NodeObjectElementT*) node);
            NodeStringT* key = object->key;
            DStringT* keyStr = key->value;
            wprintf(L"Key \"%s\"\n", keyStr->data);
            PrintNode(object->value);
            break;
        }
        case NODE_TYPE_STRING:
            wprintf(L"Value \"%s\"\n", ((NodeStringT*) node)->value->data);
            break;
    }
}

/***********************************************************************************************************************
Static function definitions
***********************************************************************************************************************/
BOOL is_char_space(wchar_t unicodeChar)
{
    return (unicodeChar == UNICODE_TABULATION || unicodeChar == UNICODE_LINE_FEED ||
            unicodeChar == UNICODE_CARRIAGE_RETURN || unicodeChar == UNICODE_SPACE);
}

void move_to_next_char(JSONParserT* parser) { parser->offset += current_char_length(parser); }

uint8_t current_char_length(JSONParserT* parser) { return utf8_get_char_length(parser->buffer[parser->offset]); }

UnicodeTokenT get_token(wchar_t unicodeChar)
{
    UnicodeTokenT token;
    switch (unicodeChar)
    {
        case UNICODE_LEFT_SQUARE_BRACKET:
            token = UNICODE_TOKEN_LEFT_SQUARE_BRACKET;
            break;
        case UNICODE_LEFT_CURLY_BRACKET:
            token = UNICODE_TOKEN_LEFT_CURLY_BRACKET;
            break;
        case UNICODE_RIGHT_SQUARE_BRACKET:
            token = UNICODE_TOKEN_RIGHT_SQUARE_BRACKET;
            break;
        case UNICODE_RIGHT_CURLY_BRACKET:
            token = UNICODE_TOKEN_RIGHT_CURLY_BRACKET;
            break;
        case UNICODE_COLON:
            token = UNICODE_TOKEN_COLON;
            break;
        case UNICODE_COMMA:
            token = UNICODE_TOKEN_COMMA;
            break;
        case UNICODE_QUOTATION_MARK:
            token = UNICODE_TOKEN_QUOTATION_MARK;
            break;
        case UNICODE_BACK_SLASH:
            token = UNICODE_TOKEN_BACK_SLASH;
            break;
        case UNICODE_t:
        case UNICODE_f:
        case UNICODE_n: {
            token = UNICODE_TOKEN_NONE;
        }
        break;
        default:
            token = UNICODE_TOKEN_NONE;
            break;
    }

    return token;
}

wchar_t current_unicode_char(JSONParserT* parser) { return utf8_to_unicode(&parser->buffer[parser->offset]); }

UnicodeTokenT get_current_token(JSONParserT* parser) { return get_token(current_unicode_char(parser)); }

void skip_spaces(JSONParserT* parser)
{

    BOOL first = TRUE;
    wchar_t currentChar = UNICODE_TABULATION;

    while (is_char_space(currentChar))
    {
        if (TRUE == first) { first = FALSE; }
        currentChar = current_unicode_char(parser);

        if (is_char_space(currentChar)) { parser->offset += current_char_length(parser); }
    }
}

NodeT* ParseValue(JSONParserT* parser)
{
    skip_spaces(parser);
    UnicodeTokenT token = get_current_token(parser);

    NodeT* result = NULL;

    switch (token)
    {
        case UNICODE_TOKEN_LEFT_CURLY_BRACKET:
            result = ParseObject(parser);
            break;
        case UNICODE_TOKEN_LEFT_SQUARE_BRACKET:
            result = ParseArray(parser);
            break;
        case UNICODE_TOKEN_QUOTATION_MARK: {
            result = ParseString(parser);
        }
        break;
    }
    return result;
}

NodeT* ParseString(JSONParserT* parser)
{
    NodeT* result = NULL;

    UnicodeTokenT token = get_current_token(parser);
    if (token == UNICODE_TOKEN_QUOTATION_MARK)
    {
        CStringViewT str;
        const int8_t* data = &parser->buffer[parser->offset + current_char_length(parser)];
        uint32_t length = 0;
        parser->offset += 1;
        token = get_current_token(parser);
        while (token != UNICODE_TOKEN_QUOTATION_MARK || token == UNICODE_TOKEN_BACK_SLASH)
        {
            if (UNICODE_TOKEN_BACK_SLASH == token && UNICODE_TOKEN_QUOTATION_MARK == get_current_token(parser))
            {
                token = UNICODE_TOKEN_NONE;
            }
            else { token = get_current_token(parser); }
            length += current_char_length(parser);
            parser->offset += current_char_length(parser);
        }
        length -= 1;

        DStringT* dStrResult = str_create(data, length);
        NodeStringT* strNode = (NodeStringT*) malloc(sizeof(NodeStringT));
        strNode->valueType = NODE_TYPE_STRING;
        strNode->value = dStrResult;
        result = strNode;

        wprintf(L"Value \"%s\"\n", strNode->value->data);
    }

    return result;
}

NodeT* ParseArray(JSONParserT* parser)
{
    NodeArrayT* result = NULL;

    move_to_next_char(parser);
    UnicodeTokenT token = UNICODE_TOKEN_FALSE;

    while (token != UNICODE_TOKEN_RIGHT_SQUARE_BRACKET && token != UNICODE_TOKEN_NONE)
    {
        skip_spaces(parser);
        NodeT* val = ParseValue(parser);
        if (result == NULL)
        {
            result = (NodeArrayT*) malloc(sizeof(NodeArrayT*));
            switch (val->valueType)
            {
                case NODE_TYPE_STRING:
                    result->elementSize = sizeof(NodeStringT*);
                    break;
                case NODE_TYPE_ARRAY:
                    result->elementSize = sizeof(NodeArrayT*);
                    break;
                case NODE_TYPE_OBJECT:
                    result->elementSize = sizeof(NodeObjectT*);
                    break;
            }
            result->valueType = NODE_TYPE_ARRAY;
            result->data = darr_create_generic(result->elementSize);
        }
        if (val->valueType == NODE_TYPE_STRING) { wprintf(L"Value \"%s\"\n", ((NodeStringT*) val)->value->data); }

        darr_push_generic(result->data, &val);
        skip_spaces(parser);

        token = get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += current_char_length(parser); }
    }
    move_to_next_char(parser);

    return (NodeT*) result;
}

NodeT* ParseObject(JSONParserT* parser)
{
    NodeObjectT* result = NULL;
    move_to_next_char(parser);

    UnicodeTokenT token = UNICODE_TOKEN_FALSE;

    result = (NodeObjectT*) malloc(sizeof(NodeObjectT));
    result->valueType = NODE_TYPE_OBJECT;
    result->elements = NULL;

    while (token != UNICODE_TOKEN_RIGHT_CURLY_BRACKET && token != UNICODE_TOKEN_NONE)
    {
        skip_spaces(parser);
        token = get_current_token(parser);
        NodeT* key = ParseString(parser);
        wprintf(L"Key \"%s\"\n", ((NodeStringT*) key)->value->data);

        token = get_current_token(parser);
        skip_spaces(parser);
        if (token != UNICODE_TOKEN_COLON) { LOG_ERROR("Expected colon after key string!\n"); }
        else { parser->offset += current_char_length(parser); }

        printf("Value:");
        NodeT* value = ParseValue(parser);

        if (result->elements == NULL) { result->elements = darr_create_generic(sizeof(NodeObjectElementT*)); }

        NodeObjectElementT* object = (NodeObjectElementT*) malloc(sizeof(NodeObjectElementT));
        object->valueType = NODE_TYPE_OBJECT_ELEMENT;
        object->key = key;
        object->value = value;
        darr_push_generic(result->elements, &object);
        skip_spaces(parser);

        token = get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += current_char_length(parser); }
    }
    move_to_next_char(parser);

    return result;
}

inline static JSONParserResultT json_parse(const char* path, JSONParserT* parser)
{
    setlocale(LC_ALL, "");
    JSONParserResultT result = JSON_PARSE_RESULT_ERROR;
    LOG_INFO("Parsing %s\n", path);

    int8_t* data;
    size_t filesize;
    FileOpResultT fileReadResult = file_read_utf8(path, &filesize, &data);

    if (FILE_READ_SUCCESFULLY != fileReadResult) { result = JSON_PARSE_RESULT_ERROR; }
    else
    {
        parser->buffer = data;
        parser->length = filesize;
        parser->offset = 0;

        parser->data = ParseValue(parser);

        printf("\n\n\n\n\n\n\n");
        PrintNode(parser->data);
    }
    return result;
}

/***********************************************************************************************************************
Static functions implementation
***********************************************************************************************************************/

#endif// JSONPARSER_HEADER