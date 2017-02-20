#include "Ts_BravoImperfections.h"

static int16_t quick_mask[3][3] =
{
	{ -1, 0, -1 },
	{ 0, 4, 0 },
	{ -1, 0, -1 }
};

Ts_BravoImperfections::Ts_BravoImperfections(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
: Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pImperfectionsTestData(NULL)
{
}

Ts_BravoImperfections::~Ts_BravoImperfections()
{
}

void Ts_BravoImperfections::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoImperfections::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoImperfections::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pImperfectionsTestData = new ImperfectionsTestData();
	_pImperfectionsTestData->data_name = _strName;

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
		_pImperfectionsTestData->m_peggedThreshold = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pImperfectionsTestData->m_flooredThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pImperfectionsTestData->m_maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
}

void Ts_BravoImperfections::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	_pImperfectionsTestData->executed = true;

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t colNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	//Retrieve FW_BL and Image
	CalibrateTestData *pCalibrateData = static_cast<CalibrateTestData*>(RetrieveTestData("Calibrate"));
	if (NULL == pCalibrateData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoImperfections::Calibrate has not executed!");
		throw Exception;
		return;
	}
	AcqImageNoFingerTestData *pAcqImageNoFingerTestData = static_cast<AcqImageNoFingerTestData*>(RetrieveTestData("AcqImgFinger"));
	if (NULL == pCalibrateData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoImperfections::AcqImgFinger has not executed!");
		throw Exception;
		return;
	}

	int16_t val1, val2;
	int16_t base;
	int32_t ix;
	int32_t sum;
	
	half_word_frame_data_t *frame1 = new half_word_frame_data_t();
	half_word_frame_data_t *frame2 = new half_word_frame_data_t();
	frame1->frame_cols = colNumber;
	frame1->frame_rows = rowNumber;
	frame2->frame_cols = colNumber;
	frame2->frame_rows = rowNumber;
	for (int i = 0; i<colNumber*rowNumber; i++)
	{
		frame1->data[i] = 32767;
		frame2->data[i] = -32768;
	}

	for (int i = 0; i<colNumber*rowNumber; i++)
	{
		val1 = pAcqImageNoFingerTestData->arrImage[i];
		if (val1 < frame1->data[i])
		{
			frame1->data[i] = val1;
		}
		if (val1 > frame2->data[i])
		{
			frame2->data[i] = val1;
		}
	}
	/* Calculate an average frame and range frame */
	for (int i = 0; i<colNumber*rowNumber; i++)
	{
		val1 = frame1->data[i]; //AVG
		val2 = frame2->data[i]; //RNG
		base = pCalibrateData->FWBaseline[i];

		frame2->data[i] = val2 - val1; /* Range = Maximum - Minimum */
		frame1->data[i] = val1 + frame2->data[i] / 2;  /* Minimum + Range/2 */

		ix = frame1->data[i] - base;
		if (ix < -32768) ix = -32768;
		if (ix > 32767) ix = 32767;
		frame1->data[i] = (int16_t)ix;
	}

	/*
	 *	Start the bubble test:
	 *		frame1->data will be the image test needs to work on.
	 *		frame2->data can be used to store gradient
	 */
	int16_t val3, val4;
	int32_t r, c, a, b, rows, cols;
	int32_t gx, gy;
	uint32_t gxx, gyy;
	rows = frame1->frame_rows;
	cols = frame1->frame_cols;
	gxx = 0;
	gyy = 0;

	//	0) Clear the buffer that will hold filtered image
	for (int i = 0; i<colNumber*rowNumber; i++)
	{
		frame2->data[i] = 0;
	}

	//	1) Convolve test image with the quick_mask kernel and
	//store results in frame2->data
	for (r = 1; r<rows - 1; r++)
	{
		for (c = 1; c<cols - 1; c++)
		{
			sum = 0;
			for (a = -1; a<2; a++) {
				for (b = -1; b<2; b++) {
					//Ethan 12/19/16: Get more data.
					sum = sum + frame1->data[(r + a)*cols + (c + b)] * quick_mask[a + 1][b + 1];
					//sum = sum + frame1->data[(r + a)*cols + (c + b)] / 100 * quick_mask[a + 1][b + 1];
				}
			}

			frame2->data[r * cols + c] = sum;
		} /* ends loop over c */
	} /* ends loop over r */
	/*End Convolution Here*/

	//	2) Find the gradient per zone and convert to a scalar value 
	/* exclude border pixels for convenience */
	for (int i = 0; i<colNumber*rowNumber; i++)
	{
		frame1->data[i] = 0;
	}

	//for (r = 2; r < rows - 2; ++r) {
	//	for (c = 2; c < cols - 2; ++c) {



	//		/* gradient */
	//		gx = (frame2->data[r*TEST_QNTY_RX + c + 1] / 50 - frame2->data[r*TEST_QNTY_RX + c - 1] / 50);
	//		gy = (frame2->data[(r + 1)*TEST_QNTY_RX + c] / 50 - frame2->data[(r - 1)*TEST_QNTY_RX + c] / 50);

	//		sum = testSqrt((uint32_t)(gx*gx) + (uint32_t)(gy*gy));

	//		frame1->data[r*cols + c] = sum;
	//	}
	//}






























	_pImperfectionsTestData->pass = true;
}

void Ts_BravoImperfections::ProcessData()
{
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pImperfectionsTestData->data_name, _pImperfectionsTestData->pass ? "Pass" : "Fail"));
	if (!(_pImperfectionsTestData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("129");
}

void Ts_BravoImperfections::CleanUp()
{
	CalculateTestTime(_pImperfectionsTestData->test_time);
	
	StoreTestData(_pImperfectionsTestData->data_name, static_cast<SynTestData*>(_pImperfectionsTestData));
}