#ifndef JSONPARSER_DEFS_HEADER
#define JSONPARSER_DEFS_HEADER
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
#include "DString.h"
#include "STDTypes.h"


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

#define UNICODE_TOKEN_NONE_STR "UNICODE_TOKEN_NONE"
#define UNICODE_TOKEN_LEFT_SQUARE_BRACKET_STR "UNICODE_TOKEN_LEFT_SQUARE_BRACKET"
#define UNICODE_TOKEN_RIGHT_SQUARE_BRACKET_STR "UNICODE_TOKEN_RIGHT_SQUARE_BRACKET"
#define UNICODE_TOKEN_LEFT_CURLY_BRACKET_STR "UNICODE_TOKEN_LEFT_CURLY_BRACKET"
#define UNICODE_TOKEN_RIGHT_CURLY_BRACKET_STR "UNICODE_TOKEN_RIGHT_CURLY_BRACKET"
#define UNICODE_TOKEN_COLON_STR "UNICODE_TOKEN_COLON"
#define UNICODE_TOKEN_COMMA_STR "UNICODE_TOKEN_COMMA"
#define UNICODE_TOKEN_QUOTATION_MARK_STR "UNICODE_TOKEN_QUOTATION_MARK"
#define UNICODE_TOKEN_TRUE_STR "UNICODE_TOKEN_TRUE"
#define UNICODE_TOKEN_FALSE_STR "UNICODE_TOKEN_FALSE"
#define UNICODE_TOKEN_NULL_STR "UNICODE_TOKEN_NULL"
#define UNICODE_TOKEN_BACK_SLASH_STR "UNICODE_TOKEN_BACK_SLASH"

/***********************************************************************************************************************
Static Variables
***********************************************************************************************************************/

static const char* tokens_str[] = {UNICODE_TOKEN_NONE_STR,
                                   UNICODE_TOKEN_LEFT_SQUARE_BRACKET_STR,
                                   UNICODE_TOKEN_RIGHT_SQUARE_BRACKET_STR,
                                   UNICODE_TOKEN_LEFT_CURLY_BRACKET_STR,
                                   UNICODE_TOKEN_RIGHT_CURLY_BRACKET_STR,
                                   UNICODE_TOKEN_COLON_STR,
                                   UNICODE_TOKEN_COMMA_STR,
                                   UNICODE_TOKEN_QUOTATION_MARK_STR,
                                   UNICODE_TOKEN_TRUE_STR,
                                   UNICODE_TOKEN_FALSE_STR,
                                   UNICODE_TOKEN_NULL_STR,
                                   UNICODE_TOKEN_BACK_SLASH_STR};

/***********************************************************************************************************************
Type Definitions
***********************************************************************************************************************/
typedef enum
{
    JSON_PARSE_RESULT_OK = 0,
    JSON_PARSE_RESULT_ERROR,
    JSON_PARSE_RESULT_FILE_NOT_FOUND
} JSONParserResultT;

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
    UNICODE_TOKEN_BACK_SLASH
} UnicodeTokenT;

typedef enum
{
    NODE_TYPE_NONE = 0,
    NODE_TYPE_OBJECT,
    NODE_TYPE_OBJECT_ELEMENT,
    NODE_TYPE_ARRAY,
    NODE_TYPE_NUMBER,
    NODE_TYPE_STRING,
    NODE_TYPE_TRUE,
    NODE_TYPE_FALSE,
    NODE_TYPE_NULL,
} ValueTypeT;

typedef struct {
    ValueTypeT valueType;
    void* data;
    void* dummy;
} NodeT;

typedef struct {
    ValueTypeT valueType;
    DStringT* value;
    void* dummy;
} NodeStringT;

typedef struct {
    ValueTypeT valueType;
    uint32_t elementSize;
    DArrayT* data;
} NodeArrayT;

typedef struct {
    ValueTypeT valueType;
    NodeStringT* key;
    NodeT* value;
} NodeObjectElementT;

typedef struct {
    ValueTypeT valueType;
    DArrayT* elements;
} NodeObjectT;

typedef struct {
    const int8_t* buffer;
    size_t length;
    uint32_t offset;

    NodeT* root;
} JSONParserT;

#endif// JSONPARSER_DEFS_HEADER