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
Static function declarations
***********************************************************************************************************************/
NodeStringT* parse_string(JSONParserT* parser);
NodeArrayT* parse_array(JSONParserT* parser);
NodeObjectT* parse_object(JSONParserT* parser);
NodeT* parse_value(JSONParserT* parser);
wchar_t current_unicode_char(JSONParserT* parser);
uint8_t current_char_length(JSONParserT* parser);
BOOL is_char_space(wchar_t current_unicode_char);
UnicodeTokenT get_token(wchar_t unicodeChar);
void move_to_next_char(JSONParserT* parser);
void free_json_tree(NodeT* node);

void print_node(NodeT* node)
{
    switch (node->valueType)
    {
        case NODE_TYPE_OBJECT: {
            NodeObjectT* object = ((NodeObjectT*) node);
            DArrayT* elements = object->elements;
            for (size_t i = 0; i < darr_length(elements); i++)
            {
                NodeT* element = (NodeT*) *(long long*) darr_get_ptr(elements, i);
                print_node(element);
            }
            break;
        }
        case NODE_TYPE_ARRAY:
            for (size_t i = 0; i < darr_length(((NodeArrayT*) node)->data); i++)
            {
                print_node((NodeT*) *(long long*) darr_get_ptr(((NodeArrayT*) node)->data, i));
            }
            break;
        case NODE_TYPE_OBJECT_ELEMENT: {
            NodeObjectElementT* object = ((NodeObjectElementT*) node);
            NodeStringT* key = object->key;
            DStringT* keyStr = key->value;
            wprintf(L"Key \"%s\"\n", keyStr->data);
            print_node(object->value);
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
void move_to_next_char(JSONParserT* parser) { parser->offset += current_char_length(parser); }

uint8_t current_char_length(JSONParserT* parser) { return utf8_get_char_length(parser->buffer[parser->offset]); }

wchar_t current_unicode_char(JSONParserT* parser) { return utf8_to_unicode(&parser->buffer[parser->offset]); }

UnicodeTokenT get_current_token(JSONParserT* parser) { return get_token(current_unicode_char(parser)); }

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

BOOL is_char_space(wchar_t unicodeChar)
{
    return (unicodeChar == UNICODE_TABULATION || unicodeChar == UNICODE_LINE_FEED ||
            unicodeChar == UNICODE_CARRIAGE_RETURN || unicodeChar == UNICODE_SPACE);
}

void skip_spaces(JSONParserT* parser)
{
    wchar_t currentChar = UNICODE_TABULATION;

    while (is_char_space(currentChar))
    {
        currentChar = current_unicode_char(parser);

        if (is_char_space(currentChar)) { parser->offset += current_char_length(parser); }
    }
}

NodeT* parse_value(JSONParserT* parser)
{
    skip_spaces(parser);
    UnicodeTokenT token = get_current_token(parser);

    NodeT* result = NULL;

    switch (token)
    {
        case UNICODE_TOKEN_LEFT_CURLY_BRACKET:
            result = (NodeT*) parse_object(parser);
            break;
        case UNICODE_TOKEN_LEFT_SQUARE_BRACKET:
            result = (NodeT*) parse_array(parser);
            break;
        case UNICODE_TOKEN_QUOTATION_MARK: {
            result = (NodeT*) parse_string(parser);
        }
        break;
    }
    return result;
}

NodeStringT* create_node_string(const int8_t* data, size_t length)
{
    DStringT* dStrResult = str_create(data, length);
    NodeStringT* strNode = (NodeStringT*) CMALLOC(sizeof(NodeStringT));
    strNode->valueType = NODE_TYPE_STRING;
    strNode->value = dStrResult;
    return strNode;
}

NodeStringT* parse_string(JSONParserT* parser)
{
    NodeStringT* result = NULL;

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

        result = create_node_string(data, length);

        wprintf(L"Value \"%s\"\n", result->value->data);
    }

    return result;
}

NodeArrayT* create_node_array()
{
    NodeArrayT* result = NULL;
    result = (NodeArrayT*) CMALLOC(sizeof(NodeArrayT*));
    result->valueType = NODE_TYPE_ARRAY;
    result->elementSize = sizeof(NodeT*);
    result->data = darr_create_generic(result->elementSize);
    return result;
}

NodeArrayT* parse_array(JSONParserT* parser)
{
    move_to_next_char(parser);

    NodeArrayT* result = create_node_array();

    UnicodeTokenT token = UNICODE_TOKEN_FALSE;
    while (token != UNICODE_TOKEN_RIGHT_SQUARE_BRACKET && token != UNICODE_TOKEN_NONE)
    {
        skip_spaces(parser);
        NodeT* val = parse_value(parser);
        if (NODE_TYPE_STRING == val->valueType) { wprintf(L"Value \"%s\"\n", ((NodeStringT*) val)->value->data); }

        darr_push_generic(result->data, &val);
        skip_spaces(parser);

        token = get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += current_char_length(parser); }
    }
    move_to_next_char(parser);

    return result;
}

NodeObjectElementT* create_object_element(NodeStringT* key, NodeT* value)
{
    NodeObjectElementT* object = (NodeObjectElementT*) CMALLOC(sizeof(NodeObjectElementT));
    object->valueType = NODE_TYPE_OBJECT_ELEMENT;
    object->key = key;
    object->value = value;
    return object;
}

NodeObjectT* create_object()
{
    NodeObjectT* result = (NodeObjectT*) CMALLOC(sizeof(NodeObjectT));
    result->valueType = NODE_TYPE_OBJECT;
    result->elements = darr_create_generic(sizeof(NodeObjectElementT*));
    return result;
}

NodeObjectT* parse_object(JSONParserT* parser)
{
    NodeObjectT* result = NULL;
    move_to_next_char(parser);

    UnicodeTokenT token = UNICODE_TOKEN_FALSE;

    result = create_object();

    while (UNICODE_TOKEN_RIGHT_CURLY_BRACKET != token && UNICODE_TOKEN_NONE != token)
    {
        skip_spaces(parser);
        NodeStringT* key = parse_string(parser);
        wprintf(L"Key \"%s\"\n", ((NodeStringT*) key)->value->data);

        skip_spaces(parser);
        if (get_current_token(parser) != UNICODE_TOKEN_COLON) { LOG_ERROR("Expected colon after key string!\n"); }
        else { parser->offset += current_char_length(parser); }

        wprintf(L"Value:");
        NodeObjectElementT* object = create_object_element(key, parse_value(parser));
        darr_push_generic(result->elements, &object);
        skip_spaces(parser);

        token = get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += current_char_length(parser); }
        skip_spaces(parser);
        token = get_current_token(parser);
    }
    move_to_next_char(parser);

    return result;
}

void free_string(NodeStringT* str)
{
    str_destroy(str->value);
    CFREE(str, sizeof(NodeStringT*));
}

void free_array(NodeArrayT* arr)
{
    for (size_t i = 0; i < darr_length(arr->data); i++)
    {
        NodeT* element = (NodeT*) *(long long*) darr_get_ptr(arr->data, i);
        free_json_tree(element);
        CFREE(element, sizeof(NodeT*));
    }
    darr_destroy(arr->data);
    CFREE(arr, sizeof(NodeArrayT));
}

void free_object_element(NodeObjectElementT* element)
{
    NodeObjectElementT* object = ((NodeObjectElementT*) element);
    free_json_tree(object->key);
    free_json_tree(object->value);
    CFREE(object, sizeof(NodeObjectElementT));
}

void free_object(NodeObjectT* object)
{
    DArrayT* elements = object->elements;
    for (size_t i = 0; i < darr_length(elements); i++)
    {
        NodeT* element = (NodeT*) *(long long*) darr_get_ptr(elements, i);
        free_json_tree(element);
    }
    darr_destroy(elements);
    CFREE(object, sizeof(NodeObjecT));
}

void free_json_tree(NodeT* node)
{
    switch (node->valueType)
    {
        case NODE_TYPE_OBJECT: {
            free_object(node);
            break;
        }
        case NODE_TYPE_ARRAY:
            free_array(node);
            break;
        case NODE_TYPE_OBJECT_ELEMENT: {
            free_object_element(node);
            break;
        }
        case NODE_TYPE_STRING:
            free_string(node);
            break;
    }
}

inline static JSONParserResultT json_parse(const char* path, JSONParserT* parser)
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

        parser->root = parse_value(parser);
    }
    return result;
}

/***********************************************************************************************************************
Static functions implementation
***********************************************************************************************************************/

#endif// JSONPARSER_HEADER