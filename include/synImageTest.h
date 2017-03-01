#pragma once

#include <stdint.h>

#ifdef SYNIMAGETEST_EXPORTS
	#define SYNIMAGINGTEST __declspec(dllexport)
#else
	#define SYNIMAGINGTEST __declspec(dllimport)
#endif

#ifdef __cplusplus
	#define SYNA_IMAGING_API extern "C" SYNIMAGINGTEST
#else 
	#define SYNA_IMAGING_API SYNIMAGINGTEST
#endif

//SYNA_IMAGING_API unsigned int synSNRTestAlpha(uint8_t *arrNoFingerImage, uint8_t *arrFingerImage, unsigned int rowNumner, unsigned int columnNumber, int * arrSingal, float *arrNoise, double * arrSNR);

SYNA_IMAGING_API unsigned int synSNRTest(int16_t *arrNoFingerImage, int16_t *arrFingerImage, unsigned int rowNumner, unsigned int columnNumber, int * arrSingal, float *arrNoise, double * arrSNR);





SYNA_IMAGING_API unsigned int synImperfectionTest(uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, int peggedThreshold, int flooredThreshold, unsigned int failLimit, int *consecutive_pegged_rows, int *consecutive_pegged_cols);