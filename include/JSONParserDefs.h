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
    UNICODE_TOKEN_NONE = 0x0000,
    UNICODE_TOKEN_LEFT_SQUARE_BRACKET = 0x005B,
    UNICODE_TOKEN_RIGHT_SQUARE_BRACKET = 0x005D,
    UNICODE_TOKEN_LEFT_CURLY_BRACKET = 0x007B,
    UNICODE_TOKEN_RIGHT_CURLY_BRACKET = 0x007D,
    UNICODE_TOKEN_COLON = 0x003A,
    UNICODE_TOKEN_COMMA = 0x002C,
    UNICODE_TOKEN_QUOTATION_MARK = 0x0022,
    UNICODE_TOKEN_TRUE = 0x0074,
    UNICODE_TOKEN_FALSE = 0x0066,
    UNICODE_TOKEN_NULL = 0x006E,
    UNICODE_TOKEN_A = 0x0041,
    UNICODE_TOKEN_T = 0x0054,
    UNICODE_TOKEN_R = 0x0052,
    UNICODE_TOKEN_U = 0x0055,
    UNICODE_TOKEN_E = 0x0045,
    UNICODE_TOKEN_F = 0x0046,
    UNICODE_TOKEN_L = 0x004C,
    UNICODE_TOKEN_N = 0x004E,
    UNICODE_TOKEN_S = 0x0053,
    UNICODE_TOKEN_BACK_SLASH = 0x005C
} UnicodeTokenT;

typedef enum
{
    UNICODE_NONE = 0x0000,
    UNICODE_TABULATION = 0x0009,
    UNICODE_CARRIAGE_RETURN = 0x000D,
    UNICODE_LINE_FEED = 0x000A,
    UNICODE_SPACE = 0x0020,
    UNICODE_COMMA = 0x002C,
    UNICODE_QUOTATION_MARK = 0x0022,
    UNICODE_LEFT_CURLY_BRACKET = 0x007B,
    UNICODE_RIGHT_CURLY_BRACKET = 0x007D,
    UNICODE_LEFT_SQUARE_BRACKET = 0x005B,
    UNICODE_RIGHT_SQUARE_BRACKET = 0x005D,
    UNICODE_COLON = 0x003A,
    UNICODE_A = 0x0041,
    UNICODE_E = 0x0045,
    UNICODE_F = 0x0046,
    UNICODE_L = 0x004C,
    UNICODE_N = 0x004E,
    UNICODE_R = 0x0052,
    UNICODE_S = 0x0053,
    UNICODE_T = 0x0054,
    UNICODE_U = 0x0055,
    UNICODE_BACK_SLASH = 0x005C
}

UnicodeCharacterT;

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
} JSONObjectT;

typedef struct {
    ValueTypeT valueType;
    DStringT* value;
    void* dummy;
} JSONStringT;

typedef struct {
    ValueTypeT valueType;
    uint32_t elementSize;
    DArrayT* data;
} JSONArrayT;

typedef struct {
    ValueTypeT valueType;
    JSONStringT* key;
    JSONObjectT* value;
} JSONObjectObjectElementT;

typedef struct {
    ValueTypeT valueType;
    DArrayT* elements;
} JSONObjectObjectT;

typedef struct {
    const int8_t* buffer;
    size_t length;
    uint32_t offset;

    JSONObjectT* root;
} JSONParserT;

#endif// JSONPARSER_DEFS_HEADER