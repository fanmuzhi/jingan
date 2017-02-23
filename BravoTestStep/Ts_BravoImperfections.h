#pragma once

#include "Syn_BravoFingerprintTest.h"

//typedef struct half_word_frame_data_s
//{
//	uint32_t   frame_cols;
//	uint32_t   frame_rows;
//	int16_t    data[BRAVO_IMAGE_RAWDATA_MAX];
//} half_word_frame_data_t;

/* Generic structure definitions */
typedef struct point_s
{
	uint8_t    x;
	uint8_t    y;
} point_t;

typedef struct area_limit_type_s
{
	point_t     low_left;
	point_t     up_right;
	uint32_t    limit_set;
} area_limit_type_t;

typedef struct bubbletest_const_params_s
{
	uint32_t			n_bubble_areas;
	area_limit_type_t   bubble_areas[QNTY_BUBBLE_CHECK_ZONES];
} bubbletest_const_params_t;

class Ts_BravoImperfections : public Syn_BravoFingerprintTest
{
public:

	Ts_BravoImperfections(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils);
	virtual ~Ts_BravoImperfections();

	virtual	void SetUp();

	virtual void Execute();

	virtual void ProcessData();

	virtual void CleanUp();

	uint32_t testSqrt(uint32_t y);

protected:

	static int16_t quick_mask[3][3];

	//static bubbletest_const_params_t bubbletest_const_params;
	static area_limit_type_t bubble_area_limit[QNTY_BUBBLE_CHECK_ZONES];

	ImperfectionsTestData *_pImperfectionsTestData;
};