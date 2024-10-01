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

/***********************************************************************************************************************
Type Definitions
***********************************************************************************************************************/

typedef enum
{
    UNICODE_TOKEN_NONE = 0,
    UNICODE_TOKEN_LEFT_SQUARE_BRACKET,
    UNICODE_TOKEN_RIGHT_SQUARE_BRACKET,
    UNICODE_TOKEN_LEFT_CURLY_BRACKET,
    UNICODE_TOKEN_RIGHT_CURLY_BRACKET,
    UNICODE_TOKEN_COLON,
    UNICODE_TOKEN_COMMA,
    UNICODE_TOKEN_QUOTATION_MARK,
    UNICODE_TOKEN_TRUE,
    UNICODE_TOKEN_FALSE,
    UNICODE_TOKEN_NULL,
} UnicodeTokenT;

typedef enum
{
    NODE_VALUE_TYPE_NONE = 0,
    NODE_VALUE_TYPE_OBJECT,
    NODE_VALUE_TYPE_ARRAY,
    NODE_VALUE_TYPE_NUMBER,
    NODE_VALUE_TYPE_STRING,
    NODE_VALUE_TYPE_TRUE,
    NODE_VALUE_TYPE_FALSE,
    NODE_VALUE_TYPE_NULL,
} ValueTypeT;

typedef struct {
    ValueTypeT valueType;
    DArrayT* values;
} ValueT;

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
                                   "UNICODE_TOKEN_NULL"};

/***********************************************************************************************************************
Static function declarations
***********************************************************************************************************************/
void ParseString(JSONParserT* parser, CStringViewT* str);
void ParseArray(JSONParserT* parser);
void ParseObject(JSONParserT* parser);
void ParseValue(JSONParserT* parser);
wchar_t current_unicode_char(JSONParserT* parser);
uint8_t current_char_length(JSONParserT* parser);
BOOL is_char_space(wchar_t current_unicode_char);
UnicodeTokenT get_token(wchar_t unicodeChar);
void move_to_next_char(JSONParserT* parser);

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

void ParseValue(JSONParserT* parser)
{
    skip_spaces(parser);
    UnicodeTokenT token = get_current_token(parser);
    switch (token)
    {
        case UNICODE_TOKEN_LEFT_CURLY_BRACKET:
            ParseObject(parser);
            break;
        case UNICODE_TOKEN_LEFT_SQUARE_BRACKET:
            ParseArray(parser);
            break;
        case UNICODE_TOKEN_QUOTATION_MARK: {
            CStringViewT key;
            ParseString(parser, &key);
            DStringT* str = string_view_to_string(&key);

            wprintf(L"\"%s\"\n", str->data);
            str_destroy(str);
        }
        break;
    }
}

void ParseString(JSONParserT* parser, CStringViewT* str)
{

    UnicodeTokenT token = get_current_token(parser);
    if (token == UNICODE_TOKEN_QUOTATION_MARK)
    {
        str->data = &parser->buffer[parser->offset + current_char_length(parser)];
        str->length = 0;
        parser->offset += 1;
        token = get_current_token(parser);
        while (token != UNICODE_TOKEN_QUOTATION_MARK)
        {
            token = get_current_token(parser);
            str->length += current_char_length(parser);
            parser->offset += current_char_length(parser);
        }
        str->length -= 1;
    }
}

void ParseArray(JSONParserT* parser)
{
    move_to_next_char(parser);
    UnicodeTokenT token = UNICODE_TOKEN_FALSE;

    while (token != UNICODE_TOKEN_RIGHT_SQUARE_BRACKET && token != UNICODE_TOKEN_NONE)
    {
        skip_spaces(parser);
        ParseValue(parser);
        skip_spaces(parser);

        token = get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += current_char_length(parser); }
    }
    move_to_next_char(parser);
}

void ParseObject(JSONParserT* parser)
{
    move_to_next_char(parser);

    UnicodeTokenT token = UNICODE_TOKEN_FALSE;

    while (token != UNICODE_TOKEN_RIGHT_CURLY_BRACKET && token != UNICODE_TOKEN_NONE)
    {
        skip_spaces(parser);
        token = get_current_token(parser);
        CStringViewT key;
        ParseString(parser, &key);
        DStringT* str = string_view_to_string(&key);

        wprintf(L"Key \"%s\"\n", str->data);
        str_destroy(str);

        token = get_current_token(parser);
        skip_spaces(parser);
        if (token != UNICODE_TOKEN_COLON) { LOG_ERROR("Expected colon after key string!\n"); }
        else { parser->offset += current_char_length(parser); }

        printf("Value:");
        ParseValue(parser);
        skip_spaces(parser);

        token = get_current_token(parser);
        if (token == UNICODE_TOKEN_COMMA) { parser->offset += current_char_length(parser); }
    }
    move_to_next_char(parser);
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
        while (parser->offset < parser->length - 1) { ParseValue(parser); }
    }
    return result;
}

/***********************************************************************************************************************
Static functions implementation
***********************************************************************************************************************/

#endif// JSONPARSER_HEADER