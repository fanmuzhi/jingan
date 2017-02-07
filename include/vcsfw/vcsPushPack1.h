/*! @file vcsPushPack1.h
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
**  USDK control structure alignment
**
**  This file pushes the current alignment setting to the internal stack
**  and set the alignment to 1.
**
**
*/

#ifdef _MSC_VER
 #ifndef VCS_OS_SGX
    #include <pshpack1.h> /*{*/
 #else
    #pragma warning(disable:4103)
    #pragma pack(push,1)
 #endif
#else
  #ifdef __GNUC__
    #pragma pack(push,1)
  #elif defined(macintosh)
    #pragma options align=packed

  #elif defined _CVI_
	    #pragma pack(push,1)
  #elif defined(__arm)
        #pragma push
        #pragma pack(1)
  #endif
#endif

