/*! @file vcsBase.h
 *******************************************************************************
 ** Android Fingerprint HAL Implementation for Synaptics Fingerprint Sensors
 **
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
**  Synaptics Fingerprint SDK Base Definitions
**
**  This file defines the OS/Platform/Architecture specifics for the given
**  build target.
**
*/

#ifndef __vcsBase_h__
#define __vcsBase_h__

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
/* include standard integer data type definitions */
#include <stdint.h>
#endif

#if defined(VCS_EFI) && defined(EFI_DEBUG)
#pragma runtime_checks( "", off )
#endif

#if (defined(VCS_OS_LINUX) && !defined(__KERNEL__)) || defined(VCS_OS_MAC)
#  include <limits.h>
/* include standard integer data type definitions */
#  include <stdint.h>
#endif /* VCS_OS_LINUX */

#if defined(__APPLE__) && defined(__MACH__)
#  define VCS_OS_MACOSX
#endif /* __APPLE__ && __MACH__ */

#if defined(VCS_OS_WINDOWS)

#  include "vcsPushWarning.h"
#  ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN          /* Exclude rarely-used stuff from Windows headers */
#  endif
#  include <windows.h>
#ifndef SKIP_TCHAR_FOR_LAB_WINDOWS
#  include <tchar.h>                   /* TCHAR and some _t routines */
#endif
#  include "vcsPopWarning.h"

/* warning C4152: '....' : nonstandard extension, function/data pointer conversion in expression */
#pragma warning(3:4152)

#elif defined (_PBA_)
#pragma data_seg( "_text" )
#pragma bss_seg( "_text" )
#pragma const_seg( "_text" )

#endif /* VCS_OS_WINDOWS */

/* include the feature definitions */
#if VCS_FEATURES_DEFINED
#include "vcsFeatures.h"
#endif

#endif /* __vcsBase_h__ */
