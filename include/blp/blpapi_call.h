/* Copyright 2012. Bloomberg Finance L.P.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:  The above copyright notice and this
 * permission notice shall be included in all copies or substantial portions of
 * the Software.  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
 * EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
// blpapi_call.h                                                      -*-C++-*-
#ifndef INCLUDED_BLPAPI_CALL
#define INCLUDED_BLPAPI_CALL

//@PURPOSE: Provide functions for dispatchtbl
//
//@DESCRIPTION: This componenet provides an interface to initialize the
// function table. [NOT TO BE INCLUDED BY APPLICATION CODE DIRECTLY]
#ifdef __cplusplus

// We only support dispatchtbl on windows currently
#if defined(_WIN32) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include <stddef.h>

#include <blpapi_dispatchtbl.h>
#include <blpapi_exception.h>

#ifndef BLPAPI_DLL_NAME

#if defined(_WIN64)
#define BLPAPI_DLL_NAME _T("blpapi3_64.dll")
#else
#define BLPAPI_DLL_NAME _T("blpapi3_32.dll")
#endif  // _WIN64
#endif  // BLPAPI_DLL_NAME

// NOTE: The following macros are used to avoid direct dependencies
// on functions that may not be available in an old dll.

#define BLPAPI_TABLE_CALL(FUNCNAME)              \
    ((g_blpapiFunctionTableSize && offsetof(blpapi_FunctionEntries_t, FUNCNAME) < g_blpapiFunctionTableSize - 1) ? \
    (g_functionEntries_p->FUNCNAME) : BLPAPI_UNSUPPORTED_CALL(FUNCNAME))

#define BLPAPI_UNSUPPORTED_CALL(FUNCNAME)                        \
    (throw UnsupportedOperationException(#FUNCNAME " not supported"), 0)

#define BLPAPI_CALL_EVENTFORMATTER_APPENDMESSAGESEQ(a1, a2, a3, a4, a5, a6)                        \
    (BLPAPI_TABLE_CALL(blpapi_EventFormatter_appendMessageSeq)(a1, a2, a3, a4, a5, a6))
#define BLPAPI_CALL_EVENTFORMATTER_APPENDRECAPMESSAGESEQ(a1, a2, a3, a4, a5)                        \
    (BLPAPI_TABLE_CALL(blpapi_EventFormatter_appendRecapMessageSeq)(a1, a2, a3, a4, a5))

namespace BloombergLP {
namespace blpapi {

#ifndef BLPAPI_BUILD

class BlpCallInit {

  public:
    BlpCallInit(blpapi_FunctionEntries_t **functionEntries_pp,
        size_t *ftblSize);
};

#define BLPAPI_CALL_INIT() \
    static blpapi_FunctionEntries_t *g_functionEntries_p = 0; \
    static size_t g_blpapiFunctionTableSize = 0; \
    static BlpCallInit g_blpCallInit(&g_functionEntries_p, \
        &g_blpapiFunctionTableSize); \

BLPAPI_CALL_INIT()

inline void blpCallInitImpl(blpapi_FunctionEntries_t** functionEntries_p,
                            size_t* ftblSize);
inline BlpCallInit::BlpCallInit(
    blpapi_FunctionEntries_t** functionEntries_p,
    size_t* ftblSize)
{
    if (*functionEntries_p == 0) {
        blpCallInitImpl(functionEntries_p, ftblSize);
    }
}

#if defined(_AIX)
inline void blpCallInitImpl(
    blpapi_FunctionEntries_t** functionEntries_p,
    size_t* ftblSize)
{
}
#elif defined(sun) || defined(__sun)
inline void blpCallInitImpl(
    blpapi_FunctionEntries_t** functionEntries_p,
    size_t* ftblSize)
{
}
#elif defined(linux) || defined(__linux)
inline void blpCallInitImpl(
    blpapi_FunctionEntries_t** functionEntries_p,
    size_t* ftblSize)
{
}
#elif defined(_WIN32) || defined(__WIN32__)
inline void blpCallInitImpl(
    blpapi_FunctionEntries_t** functionEntries_p,
    size_t* ftblSize)
{
    HMODULE hModule = ::LoadLibraryEx(BLPAPI_DLL_NAME, NULL, 0);
    if (hModule == NULL) return;

    *functionEntries_p = (blpapi_FunctionEntries_t*)
        ::GetProcAddress(hModule, "g_blpapiFunctionEntries");

    size_t *ftblSize_p = (size_t*)::GetProcAddress(hModule,
        "g_blpapiFunctionTableSize");
    if (ftblSize_p != NULL) {
        *ftblSize = *ftblSize_p;
    }
}
#endif // blpCallInitImpl os specific implmentation

#else
#if defined(_WIN32) || defined(__WIN32__)
static blpapi_FunctionEntries_t *g_functionEntries_p = 0; 
static size_t g_blpapiFunctionTableSize = 0; 
#endif // _WIN32
#endif // ifdef BLPAPI_BUILD

}  // close namespace blpapi {
}  // close namespace BloombergLP {

#else  // if defined(_WIN32) || defined(__WIN32__)
#define BLPAPI_CALL_OBSOLETE(FUNCNAME) (FUNCNAME)
#define BLPAPI_CALL_EVENTFORMATTER_APPENDMESSAGESEQ(a1, a2, a3, a4, a5, a6)                        \
    (blpapi_EventFormatter_appendMessageSeq)(a1, a2, a3, a4, a5, a6)
#define BLPAPI_CALL_EVENTFORMATTER_APPENDRECAPMESSAGESEQ(a1, a2, a3, a4, a5)                        \
    (blpapi_EventFormatter_appendRecapMessageSeq)(a1, a2, a3, a4, a5)

#endif // if defined(_WIN32) || defined(__WIN32__)

#endif // __cplusplus

#endif // INCLUDED_BLPAPI_CALL
