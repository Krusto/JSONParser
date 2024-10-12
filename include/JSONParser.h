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

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define json_get_current_char(parser) ((parser)->buffer[(parser)->offset])
#define json_current_unicode_char(parser) utf8_to_unicode(&json_get_current_char(parser))
#define json_move_to_next_char(parser) ((parser)->offset += utf8_get_char_length(json_get_current_char(parser)))
#define json_move_to_prev_char(parser) ((parser)->offset -= utf8_get_char_length(json_get_current_char(parser)))
#define json_current_char_length(parser) utf8_get_char_length(json_get_current_char(parser))
#define json_get_current_token(parser) get_token(parser)

/***********************************************************************************************************************
Static function declarations
***********************************************************************************************************************/
JSONObjectT* json_parse_value(JSONParserT* parser);
JSONStringT* json_parse_string(JSONParserT* parser);
JSONArrayT* json_parse_array(JSONParserT* parser);
JSONObjectObjectElementT* json_parse_object_element(JSONParserT* parser);
JSONObjectObjectT* json_parse_object(JSONParserT* parser);

void free_json_tree(JSONObjectT* node);
void destroy_json_parser(JSONParserT* parser);

void print_node(JSONObjectT* node, uint32_t indent)
{
    switch (node->valueType)
    {
        case NODE_TYPE_OBJECT: {
            JSONObjectObjectT* object = ((JSONObjectObjectT*) node);
            DArrayT* elements = object->elements;
            indent += 4;
            for (size_t i = 0; i < darr_length(elements); i++)
            {
                JSONObjectT* element = (JSONObjectT*) *(long long*) darr_get_ptr(elements, i);
                print_node(element, indent);
            }
            break;
        }
        case NODE_TYPE_ARRAY:
            for (size_t i = 0; i < darr_length(((JSONArrayT*) node)->data); i++)
            {
                print_node((JSONObjectT*) *(long long*) darr_get_ptr(((JSONArrayT*) node)->data, i), indent);
            }
            break;
        case NODE_TYPE_OBJECT_ELEMENT: {
            JSONObjectObjectElementT* object = ((JSONObjectObjectElementT*) node);
            JSONStringT* key = object->key;
            DStringT* keyStr = key->value;
            WLOG(L"\n");
            for (size_t i = 0; i < indent; i++) { WLOG(L" "); }
            WLOG(L"Key \"%s\" : ", keyStr->data);
            indent += 2;
            print_node(object->value, indent);
            break;
        }
        case NODE_TYPE_STRING:
            WLOG(L"\"%s\"\n", ((JSONStringT*) node)->value->data);
            break;
    }
}

/***********************************************************************************************************************
Static function definitions
***********************************************************************************************************************/
UnicodeTokenT get_token(JSONParserT* parser)
{
    wchar_t unicodeChar = json_current_unicode_char(parser);
    return unicodeChar;
}

BOOL is_char_space(wchar_t unicodeChar)
{
    return (unicodeChar == UNICODE_TABULATION || unicodeChar == UNICODE_LINE_FEED ||
            unicodeChar == UNICODE_CARRIAGE_RETURN || unicodeChar == UNICODE_SPACE);
}

void json_buffer_skip_spaces(JSONParserT* parser)
{
    wchar_t currentChar = UNICODE_TABULATION;

    while (is_char_space(currentChar))
    {
        currentChar = json_current_unicode_char(parser);

        if (is_char_space(currentChar)) { parser->offset += json_current_char_length(parser); }
    }
}

JSONObjectT* json_parse_value(JSONParserT* parser)
{
    json_buffer_skip_spaces(parser);
    UnicodeTokenT token = json_get_current_token(parser);

    JSONObjectT* result = NULL;

    switch (token)
    {
        case UNICODE_TOKEN_LEFT_CURLY_BRACKET:
            result = (JSONObjectT*) json_parse_object(parser);
            break;
        case UNICODE_TOKEN_LEFT_SQUARE_BRACKET:
            result = (JSONObjectT*) json_parse_array(parser);
            break;
        case UNICODE_TOKEN_QUOTATION_MARK: {
            result = (JSONObjectT*) json_parse_string(parser);
        }
        break;
    }
    return result;
}

JSONStringT* create_node_string(const int8_t* data, size_t length)
{
    DStringT* dStrResult = str_create(data, length);
    JSONStringT* strJSON = (JSONStringT*) CMALLOC(sizeof(JSONStringT));
    strJSON->valueType = NODE_TYPE_STRING;
    strJSON->value = dStrResult;
    return strJSON;
}

JSONStringT* json_parse_string(JSONParserT* parser)
{
    JSONStringT* result = NULL;

    UnicodeTokenT token = json_get_current_token(parser);
    if (token == UNICODE_TOKEN_QUOTATION_MARK)
    {
        CStringViewT str;
        const int8_t* data = &parser->buffer[parser->offset + json_current_char_length(parser)];
        uint32_t length = 0;
        parser->offset += 1;
        token = json_get_current_token(parser);
        while (token != UNICODE_TOKEN_QUOTATION_MARK || token == UNICODE_TOKEN_BACK_SLASH)
        {
            if (UNICODE_TOKEN_BACK_SLASH == token && UNICODE_TOKEN_QUOTATION_MARK == json_get_current_token(parser))
            {
                token = UNICODE_TOKEN_NONE;
            }
            else { token = json_get_current_token(parser); }
            length += json_current_char_length(parser);
            parser->offset += json_current_char_length(parser);
        }
        length -= 1;

        result = create_node_string(data, length);

        wprintf(L"Value \"%s\"\n", result->value->data);
    }

    return result;
}

JSONArrayT* create_node_array()
{
    JSONArrayT* result = NULL;
    result = (JSONArrayT*) CMALLOC(sizeof(JSONArrayT));
    result->valueType = NODE_TYPE_ARRAY;
    result->elementSize = sizeof(JSONObjectT*);
    result->data = darr_create_generic(result->elementSize);
    return result;
}

JSONArrayT* json_parse_array(JSONParserT* parser)
{
    json_move_to_next_char(parser);

    JSONArrayT* result = create_node_array();

    UnicodeTokenT token = UNICODE_TOKEN_FALSE;
    while (token != UNICODE_TOKEN_RIGHT_SQUARE_BRACKET && token != UNICODE_TOKEN_NONE)
    {
        json_buffer_skip_spaces(parser);
        JSONObjectT* val = json_parse_value(parser);
        if (NODE_TYPE_STRING == val->valueType) { wprintf(L"Value \"%s\"\n", ((JSONStringT*) val)->value->data); }

        darr_push_generic(result->data, &val);
        json_buffer_skip_spaces(parser);

        token = json_get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += json_current_char_length(parser); }
    }
    json_move_to_next_char(parser);

    return result;
}

JSONObjectObjectElementT* create_object_element(JSONStringT* key, JSONObjectT* value)
{
    JSONObjectObjectElementT* object = (JSONObjectObjectElementT*) CMALLOC(sizeof(JSONObjectObjectElementT));
    object->valueType = NODE_TYPE_OBJECT_ELEMENT;
    object->key = key;
    object->value = value;
    return object;
}

JSONObjectObjectT* create_object()
{
    JSONObjectObjectT* result = (JSONObjectObjectT*) CMALLOC(sizeof(JSONObjectObjectT));
    result->valueType = NODE_TYPE_OBJECT;
    result->elements = darr_create_generic(sizeof(JSONObjectObjectElementT*));
    return result;
}

JSONObjectObjectElementT* json_parse_object_element(JSONParserT* parser)
{
    JSONObjectObjectElementT* object = NULL;

    json_buffer_skip_spaces(parser);
    JSONStringT* key = json_parse_string(parser);
    wprintf(L"Key \"%s\"\n", ((JSONStringT*) key)->value->data);

    json_buffer_skip_spaces(parser);
    if (json_get_current_token(parser) != UNICODE_TOKEN_COLON) { LOG_ERROR("Expected colon after key string!\n"); }
    else { parser->offset += json_current_char_length(parser); }

    wprintf(L"Value:");
    object = create_object_element(key, json_parse_value(parser));

    return object;
}

JSONObjectObjectT* json_parse_object(JSONParserT* parser)
{
    JSONObjectObjectT* result = NULL;
    json_move_to_next_char(parser);

    UnicodeTokenT token = UNICODE_TOKEN_FALSE;

    result = create_object();

    while (UNICODE_TOKEN_RIGHT_CURLY_BRACKET != token && UNICODE_TOKEN_NONE != token)
    {
        JSONObjectObjectElementT* object = json_parse_object_element(parser);
        darr_push_generic(result->elements, &object);

        json_buffer_skip_spaces(parser);

        token = json_get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += json_current_char_length(parser); }
        json_buffer_skip_spaces(parser);
        token = json_get_current_token(parser);
    }
    json_move_to_next_char(parser);

    return result;
}

void free_string(JSONStringT* str)
{
    str_destroy(str->value);
    CFREE(str, sizeof(JSONStringT*));
}

void free_array(JSONArrayT* arr)
{
    for (size_t i = 0; i < darr_length(arr->data); i++)
    {
        JSONObjectT* element = (JSONObjectT*) *(long long*) darr_get_ptr(arr->data, i);
        free_json_tree(element);
    }
    darr_destroy(arr->data);
    CFREE(arr, sizeof(JSONArrayT));
}

void free_object_element(JSONObjectObjectElementT* element)
{
    JSONObjectObjectElementT* object = ((JSONObjectObjectElementT*) element);
    free_json_tree((JSONObjectT*) object->key);
    free_json_tree(object->value);
    CFREE(object, sizeof(JSONObjectObjectElementT));
}

void free_object(JSONObjectObjectT* object)
{
    DArrayT* elements = object->elements;
    for (size_t i = 0; i < darr_length(elements); i++)
    {
        JSONObjectT* element = (JSONObjectT*) *(long long*) darr_get_ptr(elements, i);
        free_json_tree(element);
    }
    darr_destroy(elements);
    CFREE(object, sizeof(JSONObjecT));
}

void destroy_json_parser(JSONParserT* parser)
{
    free_json_tree(parser->root);
    CFREE(parser->buffer, parser->length);
}

void free_json_tree(JSONObjectT* node)
{
    if (NULL != node)
    {
        switch (node->valueType)
        {
            case NODE_TYPE_OBJECT: {
                free_object((JSONObjectObjectT*) node);
                break;
            }
            case NODE_TYPE_ARRAY:
                free_array((JSONArrayT*) node);
                break;
            case NODE_TYPE_OBJECT_ELEMENT: {
                free_object_element((JSONObjectObjectElementT*) node);
                break;
            }
            case NODE_TYPE_STRING:
                free_string((JSONStringT*) node);
                break;
        }
    }
}

inline static JSONParserResultT json_parse_file(const char* path, JSONParserT* parser)
{
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

        parser->root = json_parse_value(parser);
    }
    return result;
}

/***********************************************************************************************************************
Static functions implementation
***********************************************************************************************************************/

#endif// JSONPARSER_HEADER