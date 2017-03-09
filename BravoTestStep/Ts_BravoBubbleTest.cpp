#include "Ts_BravoBubbleTest.h"
#include "synImageTest.h"

Ts_BravoBubbleTest::Ts_BravoBubbleTest(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
: Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pBubbleTestData(NULL)
{
}

Ts_BravoBubbleTest::~Ts_BravoBubbleTest()
{
}

void Ts_BravoBubbleTest::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoBubbleTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoBubbleTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pBubbleTestData = new BubbleTestData();
	_pBubbleTestData->data_name = _strName;

	_pBubbleTestData->m_peggedThreshold = 200;
	_pBubbleTestData->m_flooredThreshold = 20;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);
	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();

	if (iListSize < 3)
	{
		for (size_t t = 1; t <= 3 - iListSize; t++)
			listOfArgValue.push_back("");
	}

	if (0 != listOfArgValue[0].length())
		_pBubbleTestData->m_peggedThreshold = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pBubbleTestData->m_flooredThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pBubbleTestData->m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
}

void Ts_BravoBubbleTest::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	_pBubbleTestData->executed = true;

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t colNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	//Retrieve FW_BL and Image
	CalibrateTestData *pCalibrateData = static_cast<CalibrateTestData*>(RetrieveTestData("Calibrate"));
	if (NULL == pCalibrateData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoBubbleTest::Calibrate has not executed!");
		throw Exception;
		return;
	}
	AcqImageFingerTestData *pAcqImageFingerTestData = static_cast<AcqImageFingerTestData*>(RetrieveTestData("AcqImgFinger"));
	if (NULL == pCalibrateData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoBubbleTest::AcqImgFinger has not executed!");
		throw Exception;
		return;
	}

	//int16_t val1, val2;
	//int16_t base;
	//int32_t ix;
	//int32_t sum;

	//_pBubbleTestData->frame1.frame_cols = colNumber;
	//_pBubbleTestData->frame1.frame_rows = rowNumber;
	//_pBubbleTestData->frame2.frame_cols = colNumber;
	//_pBubbleTestData->frame2.frame_rows = rowNumber;
	//for (unsigned int i = 0; i<colNumber*rowNumber; i++)
	//{
	//	_pBubbleTestData->frame1.data[i] = 32767;
	//	_pBubbleTestData->frame2.data[i] = -32768;
	//}

	//for (unsigned int i = 0; i<colNumber*rowNumber; i++)
	//{
	//	val1 = pAcqImageFingerTestData->arrImage[i];
	//	if (val1 < _pBubbleTestData->frame1.data[i])
	//	{
	//		_pBubbleTestData->frame1.data[i] = val1;
	//	}
	//	if (val1 > _pBubbleTestData->frame2.data[i])
	//	{
	//		_pBubbleTestData->frame2.data[i] = val1;
	//	}
	//}
	///* Calculate an average frame and range frame */
	//for (unsigned int i = 0; i<colNumber*rowNumber; i++)
	//{
	//	val1 = _pBubbleTestData->frame1.data[i]; //AVG
	//	val2 = _pBubbleTestData->frame2.data[i]; //RNG
	//	base = pCalibrateData->FWBaseline[i];

	//	_pBubbleTestData->frame2.data[i] = val2 - val1; /* Range = Maximum - Minimum */
	//	_pBubbleTestData->frame1.data[i] = val1 + _pBubbleTestData->frame2.data[i] / 2;  /* Minimum + Range/2 */

	//	ix = _pBubbleTestData->frame1.data[i] - base;
	//	if (ix < -32768) ix = -32768;
	//	if (ix > 32767) ix = 32767;
	//	_pBubbleTestData->frame1.data[i] = (int16_t)ix;
	//}

	///*
	//*	Start the bubble test:
	//*		frame1->data will be the image test needs to work on.
	//*		frame2->data can be used to store gradient
	//*/
	////int16_t val3, val4;
	//int32_t r, c, a, b, rows, cols;
	//int32_t gx, gy;
	//uint32_t gxx, gyy;
	//rows = _pBubbleTestData->frame1.frame_rows;
	//cols = _pBubbleTestData->frame1.frame_cols;
	//gxx = 0;
	//gyy = 0;

	////	0) Clear the buffer that will hold filtered image
	//for (unsigned int i = 0; i<colNumber*rowNumber; i++)
	//{
	//	_pBubbleTestData->frame2.data[i] = 0;
	//}

	////	1) Convolve test image with the quick_mask kernel and
	////store results in frame2->data
	//for (r = 1; r<rows - 1; r++)
	//{
	//	for (c = 1; c<cols - 1; c++)
	//	{
	//		sum = 0;
	//		for (a = -1; a<2; a++) {
	//			for (b = -1; b<2; b++) {
	//				//Ethan 12/19/16: Get more data.
	//				sum = sum + _pBubbleTestData->frame1.data[(r + a)*cols + (c + b)] * quick_mask[a + 1][b + 1];
	//				//sum = sum + _pBubbleTestData->frame1.data[(r + a)*cols + (c + b)] / 100 * quick_mask[a + 1][b + 1];
	//			}
	//		}

	//		_pBubbleTestData->frame2.data[r * cols + c] = sum;
	//	} /* ends loop over c */
	//} /* ends loop over r */
	///*End Convolution Here*/

	////2) Find the gradient per zone and convert to a scalar value 
	///* exclude border pixels for convenience */
	//for (unsigned int i = 0; i<colNumber*rowNumber; i++)
	//{
	//	_pBubbleTestData->frame1.data[i] = 0;
	//}

	//for (r = 2; r < rows - 2; ++r) {
	//	for (c = 2; c < cols - 2; ++c) {
	//		/* gradient */
	//		gx = (_pBubbleTestData->frame2.data[r*colNumber + c + 1] / 50 - _pBubbleTestData->frame2.data[r*colNumber + c - 1] / 50);
	//		gy = (_pBubbleTestData->frame2.data[(r + 1)*colNumber + c] / 50 - _pBubbleTestData->frame2.data[(r - 1)*colNumber + c] / 50);

	//		sum = testSqrt((uint32_t)(gx*gx) + (uint32_t)(gy*gy));

	//		_pBubbleTestData->frame1.data[r*cols + c] = sum;
	//	}
	//}

	////frame1 is holding the gradient			

	//uint32_t nTotal;
	//uint32_t sum_unsigned = 0;
	////results->parameter[TEST_PARAMETER_BUBBLE_MEASURE_BEGIN] = 0xEFBEADDE;

	//uint32_t FailedAreaCounts = 0;
	//int32_t f;
	///* Calculate Bubble Measure */
	//for (unsigned int i = 0; i<QNTY_BUBBLE_CHECK_ZONES; i++)
	//{
	//	gx = 0;
	//	gxx = 0;
	//	nTotal = 0;

	//	for (r = bubble_area_limit[i].low_left.y; r <= bubble_area_limit[i].up_right.y; r++)
	//	{
	//		f = r * colNumber + bubble_area_limit[i].low_left.x;
	//		for (c = bubble_area_limit[i].low_left.x; c <= bubble_area_limit[i].up_right.x; c++, f++)
	//		{
	//			nTotal = _pBubbleTestData->frame1.data[f] + nTotal;
	//		}
	//	}

	//	r = bubble_area_limit[i].up_right.y - bubble_area_limit[i].low_left.y + 1;
	//	c = bubble_area_limit[i].up_right.x - bubble_area_limit[i].low_left.x + 1;

	//	sum_unsigned = (nTotal) / (r * c);

	//	_pBubbleTestData->bubble_check_data[i].nBubbleMeasure_x10 = sum_unsigned;
	//	_pBubbleTestData->bubble_check_data[i].n_pixels = (r * c);
	//	//results->parameter[TEST_PARAMETER_BUBBLE_MEASURE_X100 + i] = sum_unsigned;

	//	if (_pBubbleTestData->bubble_check_data[i].nBubbleMeasure_x10 > _pBubbleTestData->m_peggedThreshold || _pBubbleTestData->bubble_check_data[i].nBubbleMeasure_x10 < _pBubbleTestData->m_flooredThreshold)
	//	{
	//		FailedAreaCounts += 1;
	//	}
	//}

	//if (FailedAreaCounts > 0)
	//	_pBubbleTestData->pass = false;
	//else
	//	_pBubbleTestData->pass = true;


	uint32_t bubble_measure_x10[SYN_QNTY_SNR_ZONES] = {0};
	uint32_t n_pixels[SYN_QNTY_SNR_ZONES] = {0};
	try
	{
		rc = synBubbleTest(pAcqImageFingerTestData->arrImage, pCalibrateData->FWBaseline, rowNumber, colNumber, _pBubbleTestData->nBubbleMeasure_x10, _pBubbleTestData->n_pixels);
	}
	catch (...)
	{
		Exception.SetError(ERROR_TSETSTEP_ARGSLENGTH);
		Exception.SetDescription("synBubbleTest is failed!");
		throw Exception;
		return;
	}
	
	_pBubbleTestData->pass = true;
	for (unsigned i = 0; i < SYN_QNTY_SNR_ZONES; i++)
	{
		if (_pBubbleTestData->nBubbleMeasure_x10[i] < 0 || _pBubbleTestData->nBubbleMeasure_x10[i] > 1)
		{
			_pBubbleTestData->pass = false;
			break;
		}
	}
}

void Ts_BravoBubbleTest::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pBubbleTestData->data_name, _pBubbleTestData->pass ? "Pass" : "Fail"));
	if (!(_pBubbleTestData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("129");
}

void Ts_BravoBubbleTest::CleanUp()
{
	CalculateTestTime(_pBubbleTestData->test_time);

	StoreTestData(_pBubbleTestData->data_name, static_cast<SynTestData*>(_pBubbleTestData));
}

uint32_t Ts_BravoBubbleTest::testSqrt(uint32_t y)
{
	uint32_t    x_old;
	uint32_t    x_new;
	uint32_t    testy;
	uint32_t    nbits;
	uint32_t    i;

	if (0 == y)
	{
		return (0);
	}

	/* select a good starting value using binary logarithms: */
	nbits = (4 * 8);    /*  */
	for (i = 4, testy = 16;; i += 2, testy <<= 2)
	{
		if (i >= nbits || y <= testy)
		{
			x_old = (1 << (i / 2));       /* x_old = sqrt(testy) */
			break;
		}
	}

	/* x_old >= sqrt(y) */
	/* use the Babylonian method to arrive at the integer square root: */
	for (;;)
	{
		x_new = (y / x_old + x_old) / 2;
		if (x_old <= x_new)
		{
			break;
		}
		x_old = x_new;
	}

	/* make sure that the answer is right: */
	if (x_old * x_old > y || (x_old + 1) * (x_old + 1) <= y)
	{
		return (0);
	}

	return (x_old);
}

//bubbletest_const_params_t Ts_BravoImperfections::bubbletest_const_params = 
//{
//	.n_bubble_areas = QNTY_BUBBLE_CHECK_ZONES,
//	.bubble_areas[0] = { .low_left = { .y = 2, .x = 2 }, .up_right = { .y = (115 - 2), .x = (55 - 2) }, .limit_set = 0 }, // 0 = All pixels 
//	.bubble_areas[1] = { .low_left = { .y = 2, .x = 2 }, .up_right = { .y = (2 + 38 - 1), .x = (2 + 26 - 1) }, .limit_set = 1 }, // 1 = Lower Left 
//	.bubble_areas[2] = { .low_left = { .y = (2 + 38), .x = 2 }, .up_right = { .y = (2 + 38 + 37 - 1), .x = (2 + 26 - 1) }, .limit_set = 1 }, // 2 = Lower Right 
//	.bubble_areas[3] = { .low_left = { .y = (2 + 38 + 37), .x = 2 }, .up_right = { .y = (115 - 2), .x = (2 + 26 - 1) }, .limit_set = 1 }, // 3 = Middle Left  
//	.bubble_areas[4] = { .low_left = { .y = 2, .x = (2 + 26) }, .up_right = { .y = (2 + 38 - 1), .x = (55 - 2) }, .limit_set = 1 }, // 4 = Middle Right 
//	.bubble_areas[5] = { .low_left = { .y = (2 + 38), .x = (2 + 26) }, .up_right = { .y = (2 + 38 + 37 - 1), .x = (55 - 2) }, .limit_set = 1 }, // 5 = Upper Left 
//	.bubble_areas[6] = { .low_left = { .y = (2 + 38 + 37), .x = (2 + 26) }, .up_right = { .y = (115 - 2), .x = (55 - 2) }, .limit_set = 1 } // 6 = Upper Right 
//}


area_limit_type_t Ts_BravoBubbleTest::bubble_area_limit[QNTY_BUBBLE_CHECK_ZONES] =
{
	{ { 0, 0 }, { (80), (88 - 2) }, { 0 } }, // 0 = All pixels 
	{ { 2, 2 }, { (2 + 26 - 1), (2 + 38 - 1) }, { 1 } }, // 1 = Lower Left 
	{ { 2, (2 + 38) }, { (2 + 26 - 1), (2 + 38 + 37 - 1) }, { 1 } }, // 2 = Lower Right
	{ { 2, (2 + 38 + 37) }, { (2 + 26 - 1), (88 - 2) }, { 1 } }, // 3 = Middle Left
	{ { (2 + 26), 2 }, { (55 - 2), (2 + 38 - 1), }, { 1 } }, // 4 = Middle Right
	{ { (2 + 26), (2 + 38) }, { (55 - 2), (2 + 38 + 37 - 1), }, { 1 } }, // 5 = Upper Left 
	{ { (2 + 26), (2 + 38 + 37) }, { (55 - 2), (88 - 2), }, { 1 } }, // 6 = Upper Right
};

int16_t Ts_BravoBubbleTest::quick_mask[3][3] =
{
	{ -1, 0, -1 },
	{ 0, 4, 0 },
	{ -1, 0, -1 }
};