#ifndef CLOG_HEADER
#define CLOG_HEADER
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
 * CLog Header
 */


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#ifndef NO_STD_LOG
#include <stdio.h>
#include <wchar.h>
#endif
/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/

// clang-format off
#ifndef NO_STD_LOG
    #define LOG( ... ) printf( __VA_ARGS__ )
    #define LOG_ERROR( ... )                                                                                               \
        LOG( "Error: " );                                                                                                  \
        LOG( __VA_ARGS__ )
    
    #ifdef CUTILS_VERBOSE
        #define LOG_INFO( ... )                                                                                                \
            LOG( "Info: " );                                                                                                   \
            LOG( __VA_ARGS__ )
    #else   
        #define LOG_INFO( ... )
    #endif
    #ifdef NDEBUG
        #define LOG_DEBUG(...)
    #else
        #define LOG_DEBUG(...)\
            LOG("Debug: ");\
            LOG(__VA_ARGS__)
    #endif

    #define WLOG( ... ) wprintf( __VA_ARGS__ )
    #define WLOG_ERROR( ... )                                                                                               \
        WLOG( L"Error: " );                                                                                                  \
        WLOG( __VA_ARGS__ )
    
    #ifdef CUTILS_VERBOSE
        #define WLOG_INFO( ... )                                                                                                \
            WLOG( L"Info: " );                                                                                                   \
            WLOG( __VA_ARGS__ )
    #else   
        #define WLOG_INFO( ... )
    #endif
    #ifdef NDEBUG
        #define WLOG_DEBUG(...)
    #else
        #define WLOG_DEBUG(...)\
            WLOG("Debug: ");\
            WLOG(__VA_ARGS__)
    #endif

#else
    #define LOG( ... )
    #define LOG_INFO( ... )
    #define LOG_DEBUG(...)
    #define LOG_ERROR(...)
    #define WLOG( ... )
    #define WLOG_INFO( ... )
    #define WLOG_DEBUG(...)
    #define WLOG_ERROR(...)
#endif
// clang-format on
/***********************************************************************************************************************
Static functions implementation
***********************************************************************************************************************/

#endif// CLOG_HEADER