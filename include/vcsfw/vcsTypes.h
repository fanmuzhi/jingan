/*! @file vcsTypes.h
 *******************************************************************************
 ** 
 ** 		NDA AND NEED-TO-KNOW REQUIRED
 ** 
 *****************************************************************************
 ** 
 **  Copyright ?006-2016 Synaptics Incorporated. All rights reserved.
 ** 
 ** 
 ** This file contains information that is proprietary to Synaptics
 ** Incorporated (“Synaptics?. The holder of this file shall treat all
 ** information contained herein as confidential, shall use the
 ** information only for its intended purpose, and shall not duplicate,
 ** disclose, or disseminate any of this information in any manner unless
 ** Synaptics has otherwise provided express, written permission.
 ** Use of the materials may require a license of intellectual property
 ** from a third party or from Synaptics. Receipt or possession of this
 ** file conveys no express or implied licenses to any intellectual
 ** property rights belonging to Synaptics.
 ** 
 **  
 ** INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,?AND
 ** SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 ** INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 ** PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 ** INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 ** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 ** CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE OF
 ** THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND BASED
 ** ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 ** NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED
 ** OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT
 ** JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY
 ** OTHER DAMAGES, SYNAPTICS?TOTAL CUMULATIVE LIABILITY TO ANY PARTY
 **  SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
*/


/*!
*******************************************************************************
**  Synaptics Fingerprint SDK Data Types
**
**  This header file contains type definitions for "fixed" size integers
**  and other simple data types used in Synaptics Fingerprint SDK.
**
** 
*/

#ifndef __vcsTypes_h__
#define __vcsTypes_h__

#include "vcsBase.h"

/*!
*******************************************************************************
**  Unsigned 8-bit integer
**
**  Unsigned integer type of at least 8 bits.
**
**  @note   When creating a variable designed to hold a character string,
**          do NOT use vcsUint8_t or vcsInt8_t. Instead use the
**          vcsChar_t and vcsString_t types.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef uint8_t vcsUint8_t;
#else
typedef unsigned char vcsUint8_t;
#endif

/*!
*******************************************************************************
**  Signed 8-bit integer
**
**  Signed integer type of at least 8 bits.
**
**  @note   When creating a variable designed to hold a character string,
**          do NOT use vcsUint8_t or vcsInt8_t. Instead use the
**          vcsChar_t and vcsString_t types.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef int8_t vcsInt8_t;
#else
typedef signed char vcsInt8_t;
#endif

/*!
*******************************************************************************
**  Unsigned 16-bit integer
**
**  Unsigned integer type of at least 16 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef uint16_t vcsUint16_t;
#else
typedef unsigned short vcsUint16_t;
#endif

/*!
*******************************************************************************
**  Signed 16-bit integer
**
**  Signed integer type of at least 16 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef int16_t vcsInt16_t;
#else
typedef signed short vcsInt16_t;
#endif

/*!
*******************************************************************************
**  Unsigned 32-bit integer
**
**  Unsigned integer type of at least 32 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

typedef uint32_t vcsUint32_t;

#else /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

#if defined(VCS_OS_WINDOWS)
typedef UINT32 vcsUint32_t;
#else /* VCS_OS_WINDOWS */
#if defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4)
typedef unsigned long vcsUint32_t;
#else /* defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4) */
typedef unsigned int vcsUint32_t;
#endif /* defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4) */
#endif /* VCS_OS_WINDOWS */

#endif /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

/*!
*******************************************************************************
**  Signed 32-bit integer
**
**  Signed integer type of at least 32 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

typedef int32_t vcsInt32_t;

#else /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

#if defined(VCS_OS_WINDOWS)
typedef INT32 vcsInt32_t;
#else /* VCS_OS_WINDOWS */
#if defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4)
typedef signed long vcsInt32_t;
#else /* defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4) */
typedef signed int vcsInt32_t;
#endif /* defined(VCS_SIZEOF_INT) && (VCS_SIZEOF_INT < 4) */
#endif /* VCS_OS_WINDOWS */

#endif /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

/*!
*******************************************************************************
**  Unsigned 64-bit integer
**
**  Unsigned integer type of at least 64 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef uint64_t vcsUint64_t;
#else
#if defined(VCS_OS_WINDOWS)
typedef UINT64 vcsUint64_t;
#else /* VCS_OS_WINDOWS */
typedef unsigned long long vcsUint64_t;
#endif /* VCS_OS_WINDOWS */
#endif

/*!
*******************************************************************************
**  Signed 64-bit integer
**
**  Signed integer type of at least 64 bits.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
typedef int64_t vcsInt64_t;
#else
#if defined(VCS_OS_WINDOWS)
typedef INT64 vcsInt64_t;
#else /* VCS_OS_WINDOWS */
typedef signed long long vcsInt64_t;
#endif /* VCS_OS_WINDOWS */
#endif

/*!
*******************************************************************************
**  Unsigned integer for pointer storage
**
**  Unsigned integer type of size enough to hold a pointer.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

typedef uintptr_t vcsUintPtr_t;

#else /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

#if defined(VCS_OS_WINDOWS) && !defined(VCS_OS_WINCE)
#ifndef VCS_OS_RTOS32
typedef uintptr_t vcsUintPtr_t;
#endif /* !VCS_OS_RTOS32 */
#else /* defined(VCS_OS_WINDOWS) && !defined(VCS_OS_WINCE) */
#if defined(ULONG_MAX) && (ULONG_MAX > 4294967295UL)
typedef unsigned long vcsUintPtr_t;
#else /* defined(ULONG_MAX) && (ULONG_MAX > 4294967295UL) */
typedef unsigned int vcsUintPtr_t;
#endif /* defined(ULONG_MAX) && (ULONG_MAX > 4294967295UL) */
#endif /* defined(VCS_OS_WINDOWS) && !defined(VCS_OS_WINCE) */

#endif /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

/*!
*******************************************************************************
**  Signed integer for pointer storage
**
**  Signed integer type of size enough to hold a pointer.
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)

typedef intptr_t vcsIntPtr_t;

#else /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

#if defined(VCS_OS_WINDOWS) && !defined(VCS_OS_WINCE)
#ifndef VCS_OS_RTOS32
typedef intptr_t vcsIntPtr_t;
#endif /* !VCS_OS_RTOS32 */
#else /* defined(VCS_OS_WINDOWS) && !defined(VCS_OS_WINCE) */
#if defined(ULONG_MAX) && (ULONG_MAX > 4294967295UL)
typedef signed long vcsIntPtr_t;
#else /* defined(ULONG_MAX) && (ULONG_MAX > 4294967295UL) */
typedef signed int vcsIntPtr_t;
#endif /* defined(ULONG_MAX) && (ULONG_MAX > 4294967295UL) */
#endif /* defined(VCS_OS_WINDOWS) && !defined(VCS_OS_WINCE) */

#endif /* defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) */

/*!
*******************************************************************************
**  Boolean type
**
**  Boolean type. This is the same as an unsigned 32-bit
**  integer, as C has no real concept of a boolean type.
*/
typedef vcsUint32_t vcsBool_t;
typedef vcsUint32_t vcsBool32_t;
typedef vcsUint32_t vcsTemplateId_t;

#if defined(VCS_TRUE)
  #undef VCS_TRUE
#endif /* VCS_TRUE */
#if defined(VCS_FALSE)
  #undef VCS_FALSE
#endif /* VCS_FALSE */
#define VCS_TRUE        ((vcsBool_t) 1) /**< VCS version of TRUE  */
#define VCS_FALSE       ((vcsBool_t) 0) /**< VCS version of FALSE */

/*!
*******************************************************************************
**  Definition of ASCII character
**
**  The definition of what constitutes an ASCII character in VCS.
**  ASCII characters are 8-bits only.
*/
typedef char vcsAsciiChar_t;

/*!
*******************************************************************************
**  Definition of Unicode character
**
**  The definition of what constitutes an Unicode character in VCS.
**  Unicode characters are 16-bits.
*/
#if defined(VCS_OS_WINDOWS)
typedef WCHAR vcsUnicodeChar_t;
#else
typedef unsigned short vcsUnicodeChar_t;
#endif
/*!
*******************************************************************************
**  Definition of "text" character
**
**  The definition of what constitutes a "text" character in VCS. If UNICODE
**  is not required (most platforms) this will just be a "char".
*/
#if defined(VCS_OS_WINDOWS)
typedef TCHAR vcsChar_t;
#else
typedef char vcsChar_t;
#endif

/*!
*******************************************************************************
**  Definition of "text string"
**
**  The definition of what constitutes a "text string" in VCS. If UNICODE
**  is not required (most platforms) this will just be a pointer to a "char".
*/
typedef vcsChar_t * vcsString_t;
typedef const vcsChar_t * vcsConstString_t;

/*!
*******************************************************************************
**  Definition of a public VCS API
**
**  The following symbol is put on the prototype of all API functions. It
**  is used to explicitly specify the calling convention for all public
**  functions.
*/
#if defined(VCS_OS_WINDOWS)

#  define VCS_STDCALL __stdcall
#  if defined(VCS_IS_DLL)
#    if defined(VCS_BUILDING_DLL)
#      define VCS_API __declspec(dllexport) __stdcall
#    else /* VCS_BUILDING_DLL */
#      define VCS_API __declspec(dllimport) __stdcall
#    endif /* VCS_BUILDING_DLL */
#  else /* VCS_IS_DLL */
#    define VCS_API __stdcall
#  endif /* VCS_IS_DLL */

#else /* VCS_OS_WINDOWS */

# if defined(WIN_WRAPPER)
#   define VCS_API __stdcall
#   define VCS_STDCALL __stdcall
# else /* WIN_WRAPPER */
#  define VCS_API
#  define VCS_STDCALL
# endif /* WIN_WRAPPER */

#endif /* VCS_OS_WINDOWS */

/*!
*******************************************************************************
**  Declare a string constant
**
**  This macro is used to declare a string constant. This will take care of
**  UNICODE vs ASCII requirements on the platform.
*/
#if defined(VCS_OS_WINDOWS)
#  define VCS_STRING(__str)       _T(__str)
#else
#  define VCS_STRING(__str)       __str
#endif

/*!
*******************************************************************************
**  Declare a character constant
**
**  This macro is used to declare a character constant. This will take care
**  of UNICODE vs ASCII requirements on the platform.
*/
#if defined(VCS_OS_WINDOWS)
#  define VCS_CHAR(__char)        _T(__char)
#else
#  define VCS_CHAR(__char)        __char
#endif

/*!
*******************************************************************************
**  Pointer is VCS_NULL
**
**  This macro determines whether a pointer is VCS_NULL or not.
*/
#define VCS_ISNULL(__ptr)         (VCS_NULL == (__ptr))
#if defined(VCS_NULL)
  #undef VCS_NULL
#endif /* VCS_NULL */

#ifndef VCS_NULL
#ifdef __cplusplus
#define VCS_NULL    0
#else /* __cplusplus */
#define VCS_NULL                  ((void *)0)
#endif /* __cplusplus */
#endif /* VCS_NULL */

/*!
*******************************************************************************
**  Number of elements in an array
**
**  This macro returns the number of elements in an array.
*/
#define VCS_NELEM(__arr)          (sizeof(__arr) / sizeof(__arr[0]))


//Disable "pointer-to-int-cast" and "unused-function" warnings for android builds.
#if defined (VCS_OS_LINUX) || defined(VCS_OS_GP)
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#endif

/*!
*******************************************************************************
**  Byte offset of a field of a structure
**
**  This macro returns the number of bytes from the beginning of the
**  structure to the to the specified field.
**  Note: For 64-bit platforms direct casting from pointer to DWORD cause
**  compiler warnings. Hence, pointer is cased to QWORD first then it is
**  truncated to DWORD.
*/
#define VCS_OFFSETOF(_s_, _f_)      (vcsUint32_t) ((vcsUint64_t) &(((_s_ *)0)->_f_))

#define VCS_OFFSETPAST(_s_, _f_)   (VCS_OFFSETOF(_s_, _f_) + sizeof(((_s_ *) 0)->_f_))

/*!
*******************************************************************************
**  VCS API handle to opaque data
**
**  This definition is used as a handle to opaque data. This is used throughout
**  the VCS interface to point to perform data hiding.
*/
typedef void * vcsHandle_t;

/*!
*******************************************************************************
**  VCS generic blob data
**
*/
typedef struct vcsBlobData_s
{
    vcsUint32_t     length;
    vcsUint8_t *    pData;
} vcsBlobData_t;

/*!
*******************************************************************************
**  VCS generic constant blob data
**
*/
typedef struct vcsConstBlobData_s
{
    vcsUint32_t         length;
    const vcsUint8_t *  pData;
} vcsConstBlobData_t;

/*!
*******************************************************************************
**  VCS maximum path length
**
**  Maximum length of any path.
*/
#if defined(VCS_OS_WINDOWS)
  #define VCS_MAX_PATH            MAX_PATH
#else /* VCS_OS_WINDOWS */
  #if defined (VCS_OS_LINUX)
   #define VCS_MAX_PATH           260
  #else /* VCS_OS_LINUX */
     #if defined(PATH_MAX)
      #define VCS_MAX_PATH        PATH_MAX
     #else /* PATH_MAX */
      #define VCS_MAX_PATH        512
     #endif /* PATH_MAX */
  #endif /* VCS_OS_LINUX */
#endif /* VCS_OS_WINDOWS */

/*!
*******************************************************************************
**  VCS path separator
*/
#define VCS_PATH_SEPERATOR        VCS_STRING("/")

/*!
*******************************************************************************
**  VCS packed type qualifier
*/
#if defined(_WIN32)
  /* Windows use #pragma pack */
  #define VCS_PACKED
#else /* _WIN32 */
  #ifdef __GNUC__
    /* GNU C use #pragma pack */
    #define VCS_PACKED
  #else /* __GNUC__ */
    #define VCS_PACKED
  #endif /* __GNUC__ */
#endif /* _WIN32 */

#ifdef USE_ROPI
    #define VCS_CONST_GLOBAL
#else /* USE_ROPI */
    #define VCS_CONST_GLOBAL    const
#endif /* USE_ROPI */

#endif /* __vcsTypes_h__ */
