#pragma once

#include <stdint.h>

#define SYN_QNTY_SNR_ZONES	7
#define SYN_MAXROW			200
#define SYN_MAXCOL			200


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


SYNA_IMAGING_API unsigned int synSNRTestAlpha(uint8_t *arrNoFingerImage, uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, int * arrSingal, float *arrNoise, double * arrSNR);

SYNA_IMAGING_API unsigned int synSNRTest(int16_t *arrNoFingerImage, int16_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, unsigned int frameNumbers, int arrSingal[SYN_QNTY_SNR_ZONES], float arrNoise[SYN_QNTY_SNR_ZONES], double arrSNR[SYN_QNTY_SNR_ZONES]);

SYNA_IMAGING_API unsigned int synImperfectionTest(uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, int peggedThreshold, int flooredThreshold, unsigned int failLimit, int *consecutive_pegged_rows, int *consecutive_pegged_cols);

SYNA_IMAGING_API unsigned int synSharpnessTest(uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, int Gx[SYN_MAXROW][SYN_MAXCOL], int Gy[SYN_MAXROW][SYN_MAXCOL], int h[SYN_MAXCOL], int g[SYN_MAXROW][SYN_MAXCOL], int pImg_t[SYN_MAXROW][SYN_MAXCOL], float SHARPNESS[4], float *percent);

SYNA_IMAGING_API unsigned int synBubbleTest(int16_t *finger_image, int16_t *fimware_baseline, unsigned int rowNumber, unsigned int columnNumber, uint32_t bubble_measure_x10[SYN_QNTY_SNR_ZONES], uint32_t n_pixels[SYN_QNTY_SNR_ZONES]);