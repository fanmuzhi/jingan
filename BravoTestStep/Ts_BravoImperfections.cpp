#include "Ts_BravoImperfections.h"

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

	_pImperfectionsTestData->peggedThreshold = 138;
	_pImperfectionsTestData->flooredThreshold = 118;
	_pImperfectionsTestData->maxAdjacentPixelsAllowed = 10;

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
		_pImperfectionsTestData->peggedThreshold = atoi(listOfArgValue[0].c_str());
	if (0 != listOfArgValue[1].length())
		_pImperfectionsTestData->flooredThreshold = atoi(listOfArgValue[1].c_str());
	if (0 != listOfArgValue[2].length())
		_pImperfectionsTestData->maxAdjacentPixelsAllowed = atoi(listOfArgValue[2].c_str());
}

void Ts_BravoImperfections::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	_pImperfectionsTestData->executed = true;

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	//Retrieve FW_BL and Image
	CalibrateTestData *pCalibrateData = static_cast<CalibrateTestData*>(RetrieveTestData("Calibrate"));
	if (NULL == pCalibrateData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoImperfections::Calibrate has not executed!");
		throw Exception;
		return;
	}
	AcqImageFingerTestData *pAcqImageFingerTestData = static_cast<AcqImageFingerTestData*>(RetrieveTestData("AcqImgFinger"));
	if (NULL == pCalibrateData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoImperfections::AcqImgFinger has not executed!");
		throw Exception;
		return;
	}

	int peggedThreshold = _pImperfectionsTestData->peggedThreshold;
	int flooredThreshold = _pImperfectionsTestData->flooredThreshold;
	unsigned int failLimit = _pImperfectionsTestData->maxAdjacentPixelsAllowed;

	_pImperfectionsTestData->pass = true;

	uint8_t *arr8bitsImage = new uint8_t[rowNumber*columnNumber];
	bpp16tobpp8(pAcqImageFingerTestData->arrImage, arr8bitsImage, rowNumber, columnNumber);

	unsigned int i, j, temp = 0;
	int countRow, countCol; //these are filled out by structure from .cpp file.
	int temp_sum = 0;
	int temp_val = 0;
	int temp_consecutive[MAXCOL] = { 0 };
	int temp_col[MAXCOL] = { 0 };
	int temp_row[MAXROW] = { 0 };

	//initialize temp_consecutive
	for (i = 0; i<columnNumber; i++)
		temp_consecutive[i] = -1;
	//init. temp_row and temp_col
	for (i = 0; i<columnNumber; i++)
		temp_col[i] = 0;
	for (i = 0; i<rowNumber; i++)
		temp_row[i] = 0;

	countRow = 0;
	countCol = 0;
	//scan through each row.
	int watchdog = 0;
	for (i = 0; i<rowNumber; i++)
	{
		for (j = 0; j<columnNumber; j++)
		{
			if ((arr8bitsImage[watchdog] >= flooredThreshold) && (arr8bitsImage[watchdog] <= peggedThreshold))
			{
				temp++;
				temp_consecutive[j] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[j] = temp;
			}

			watchdog++;

			if (temp > failLimit)//fail the test if the count is greater than our expected limits.
				_pImperfectionsTestData->pass = false;
		}
		temp_row[i] = max_array(&temp_consecutive[0], columnNumber);

		temp = 0;
	}

	//fill struct for logging purposes
	for (i = 0; i<rowNumber; i++)
		_pImperfectionsTestData->consecutive_pegged_rows[i] = temp_row[i];

	//initialize temp_consecutive
	for (i = 0; i<columnNumber; i++)
		temp_consecutive[i] = -1;
	//init. temp_row and temp_col
	for (i = 0; i<columnNumber; i++)
		temp_col[i] = 0;
	for (i = 0; i<rowNumber; i++)
		temp_row[i] = 0;
	temp = 0;
	//scan through each col.
	watchdog = 0;
	for (j = 0; j<columnNumber; j++)
	{
		for (i = 0; i<rowNumber; i++)
		{
			if ((arr8bitsImage[watchdog] >= flooredThreshold) && (arr8bitsImage[watchdog] <= peggedThreshold))
			{
				temp++;
				temp_consecutive[i] = temp;
			}
			else
			{
				temp = 0;
				temp_consecutive[i] = temp;
			}

			watchdog++;

			if (temp > failLimit)//fail the test if the count is greater than our expected limits.
				_pImperfectionsTestData->pass = false;
		}

		temp_col[j] = max_array(&temp_consecutive[0], rowNumber);
		temp = 0;
	}

	//fill struct for logging purposes
	for (i = 0; i<columnNumber; i++)
		_pImperfectionsTestData->consecutive_pegged_cols[i] = temp_col[i];

	delete[] arr8bitsImage;
	arr8bitsImage = NULL;
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

int Ts_BravoImperfections::max_array(int a[], int num_elements)
{
	int i, max = -32000;
	for (i = 0; i<num_elements; i++)
	{
		if (a[i]>max)
		{
			max = a[i];
		}
	}
	return(max);
}