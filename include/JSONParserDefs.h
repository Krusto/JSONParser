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

    NodeT* data;
} JSONParserT;

#endif// JSONPARSER_DEFS_HEADER