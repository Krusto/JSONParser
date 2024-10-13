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
    UNICODE_TOKEN_QUOTATION_MARK = 0x0022,
    UNICODE_TOKEN_COMMA = 0x002C,
    UNICODE_TOKEN_COLON = 0x003A,
    UNICODE_TOKEN_LEFT_SQUARE_BRACKET = 0x005B,
    UNICODE_TOKEN_BACK_SLASH = 0x005C,
    UNICODE_TOKEN_RIGHT_SQUARE_BRACKET = 0x005D,
    UNICODE_TOKEN_A = 0x0061,
    UNICODE_TOKEN_B = 0x0062,
    UNICODE_TOKEN_C = 0x0063,
    UNICODE_TOKEN_D = 0x0064,
    UNICODE_TOKEN_E = 0x0065,
    UNICODE_TOKEN_F = 0x0066,
    UNICODE_TOKEN_G = 0x0067,
    UNICODE_TOKEN_H = 0x0068,
    UNICODE_TOKEN_I = 0x0069,
    UNICODE_TOKEN_J = 0x006A,
    UNICODE_TOKEN_K = 0x006B,
    UNICODE_TOKEN_L = 0x006C,
    UNICODE_TOKEN_M = 0x006D,
    UNICODE_TOKEN_N = 0x006E,
    UNICODE_TOKEN_O = 0x006F,
    UNICODE_TOKEN_P = 0x0070,
    UNICODE_TOKEN_Q = 0x0071,
    UNICODE_TOKEN_R = 0x0072,
    UNICODE_TOKEN_S = 0x0073,
    UNICODE_TOKEN_T = 0x0074,
    UNICODE_TOKEN_U = 0x0075,
    UNICODE_TOKEN_V = 0x0076,
    UNICODE_TOKEN_W = 0x0077,
    UNICODE_TOKEN_X = 0x0078,
    UNICODE_TOKEN_Y = 0x0079,
    UNICODE_TOKEN_Z = 0x007A,
    UNICODE_TOKEN_LEFT_CURLY_BRACKET = 0x007B,
    UNICODE_TOKEN_RIGHT_CURLY_BRACKET = 0x007D,
    UNICODE_TOKEN_ALL
} JSONTokenT;

typedef enum
{
    UNICODE_NONE = 0x0000,
    UNICODE_TABULATION = 0x0009,
    UNICODE_LINE_FEED = 0x000A,
    UNICODE_CARRIAGE_RETURN = 0x000D,
    UNICODE_SPACE = 0x0020,
    UNICODE_QUOTATION_MARK = 0x0022,
    UNICODE_COMMA = 0x002C,
    UNICODE_COLON = 0x003A,
    UNICODE_LEFT_SQUARE_BRACKET = 0x005B,
    UNICODE_BACK_SLASH = 0x005C,
    UNICODE_RIGHT_SQUARE_BRACKET = 0x005D,
    UNICODE_LOWERCASE_A = 0x0061,
    UNICODE_LOWERCASE_B = 0x0062,
    UNICODE_LOWERCASE_C = 0x0063,
    UNICODE_LOWERCASE_D = 0x0064,
    UNICODE_LOWERCASE_E = 0x0065,
    UNICODE_LOWERCASE_F = 0x0066,
    UNICODE_LOWERCASE_G = 0x0067,
    UNICODE_LOWERCASE_H = 0x0068,
    UNICODE_LOWERCASE_I = 0x0069,
    UNICODE_LOWERCASE_J = 0x006A,
    UNICODE_LOWERCASE_K = 0x006B,
    UNICODE_LOWERCASE_L = 0x006C,
    UNICODE_LOWERCASE_M = 0x006D,
    UNICODE_LOWERCASE_N = 0x006E,
    UNICODE_LOWERCASE_O = 0x006F,
    UNICODE_LOWERCASE_P = 0x0070,
    UNICODE_LOWERCASE_Q = 0x0071,
    UNICODE_LOWERCASE_R = 0x0072,
    UNICODE_LOWERCASE_S = 0x0073,
    UNICODE_LOWERCASE_T = 0x0074,
    UNICODE_LOWERCASE_U = 0x0075,
    UNICODE_LOWERCASE_V = 0x0076,
    UNICODE_LOWERCASE_W = 0x0077,
    UNICODE_LOWERCASE_X = 0x0078,
    UNICODE_LOWERCASE_Y = 0x0079,
    UNICODE_LOWERCASE_Z = 0x007A,
    UNICODE_LEFT_CURLY_BRACKET = 0x007B,
    UNICODE_RIGHT_CURLY_BRACKET = 0x007D,
} UnicodeCharacterT;

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
    size_t offset;

    BOOL verboseOutput;

    JSONObjectT* root;
} JSONParserT;

#endif// JSONPARSER_DEFS_HEADER