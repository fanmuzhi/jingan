#include "Ts_BravoSharpness.h"
#include "synImageTest.h"

//sharpness test kernels
static int m_kernelSharpnessGx[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
static int m_kernelSharpnessGy[3][3] = { { 1, 0, -1 }, { 2, 0, -2 }, { 1, 0, -1 } };

Ts_BravoSharpness::Ts_BravoSharpness(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
{
}


Ts_BravoSharpness::~Ts_BravoSharpness()
{
}

void Ts_BravoSharpness::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoSNRTest::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoSNRTest::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	_pSharpnessData = new SharpnessData();
	_pSharpnessData->data_name = _strName;
	_pSharpnessData->limit = 30;

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (0 != iListSize)
	{
		_pSharpnessData->limit = std::stoi(listOfArgValue[0]);
	}
}

void Ts_BravoSharpness::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	uint32_t rowNumber = _pSynDutUtils->Config_MT_Info.rowNumber;
	uint32_t columnNumber = _pSynDutUtils->Config_MT_Info.columnNumber;

	int C0 = columnNumber / 3;
	int C1 = C0 + C0;
	int C2 = columnNumber;

	float measure;		//the result

	//get 8bit images
	AcqImageNoFingerTestData *pAcqImageNoFingerTestData = static_cast<AcqImageNoFingerTestData*>(RetrieveTestData("AcqImgNoFinger"));
	if (NULL == pAcqImageNoFingerTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoSNRTest::AcqImgNoFinger is not excuted!");
		throw Exception;
		return;
	}
	AcqImageFingerTestData *pAcqImageFingerTestData = static_cast<AcqImageFingerTestData*>(RetrieveTestData("AcqImgFinger"));
	if (NULL == pAcqImageFingerTestData)
	{
		Exception.SetError(ERROR_TSETSTEP_UNEXCUTED);
		Exception.SetDescription("BravoSNRTest::AcqImgFinger is not excuted!");
		throw Exception;
		return;
	}


	unsigned int frameNumbers = pAcqImageNoFingerTestData->numFrame;
	
	float fingerImage[MAXCOL][MAXROW] = {0};
	uint8_t *arr8bitsFrameFinger = new uint8_t[rowNumber*columnNumber];
	bpp16tobpp8(pAcqImageFingerTestData->arrImage, arr8bitsFrameFinger, rowNumber, columnNumber);

	//get sharpness overall 
	for (unsigned int i = 0; i < (rowNumber * columnNumber); i++)
	{
		fingerImage[i / columnNumber][i % columnNumber] = arr8bitsFrameFinger[i];
	}

	this->get_sharpness(rowNumber, columnNumber, fingerImage, &measure, _pSharpnessData);
	_pSharpnessData->SHARPNESS[3] = measure;

	//get sharpness zone1
	float tempImage[MAXCOL][MAXROW] = {0};
	for (unsigned int i = 0; i<rowNumber; i++)
	{
		for (int j = 0; j<C0; j++)
		{
			tempImage[i][j] = fingerImage[i][j];
		}
	}
	this->get_sharpness(rowNumber, C0+1, tempImage, &measure, _pSharpnessData);
	_pSharpnessData->SHARPNESS[0] = measure;


	//get sharpness zone2
	for (unsigned int i = 0; i<rowNumber; i++)
	{
		for (int j = C0; j<C1; j++)
		{
			tempImage[i][j - C0] = fingerImage[i][j];
		}
	}
	this->get_sharpness(rowNumber, C1-C0+1, tempImage, &measure, _pSharpnessData);
	_pSharpnessData->SHARPNESS[1] = measure;

	//get sharpness zone3
	for (unsigned int i = 0; i<rowNumber; i++)
	{
		for (int j = C1; j<C2; j++)
		{
			tempImage[i][j - C1] = fingerImage[i][j];
		}
	}
	this->get_sharpness(rowNumber, C2-C1+1, tempImage, &measure, _pSharpnessData);
	_pSharpnessData->SHARPNESS[2] = measure;

	float zone[3] = { 0 };
	zone[0] = _pSharpnessData->SHARPNESS[0];
	zone[1] = _pSharpnessData->SHARPNESS[1];
	zone[2] = _pSharpnessData->SHARPNESS[2];

	int min = find_min(zone, 3);
	int max = find_max(zone, 3);

	_pSharpnessData->percent = (float)abs((int)(((zone[max] - zone[min]) / _pSharpnessData->SHARPNESS[3]) * 100));

	_pSharpnessData->executed = true;

	rc = synSharpnessTest(arr8bitsFrameFinger, rowNumber, columnNumber, _pSharpnessData->Gx, _pSharpnessData->Gy, _pSharpnessData->h, _pSharpnessData->g, _pSharpnessData->pImg_t, _pSharpnessData->SHARPNESS, &(_pSharpnessData->percent));

	delete[] arr8bitsFrameFinger;
	arr8bitsFrameFinger = NULL;
}

void Ts_BravoSharpness::ProcessData()
{
	if (_pSharpnessData->percent > _pSharpnessData->limit)
	{
		_pSharpnessData->pass = false;
	}
	else
	{
		_pSharpnessData->pass = true;
	}

	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pSharpnessData->data_name, _pSharpnessData->pass ? "Pass" : "Fail"));

}

void Ts_BravoSharpness::CleanUp()
{
	CalculateTestTime(_pSharpnessData->test_time);
	if (!(_pSharpnessData->pass))
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("126");

	StoreTestData(_pSharpnessData->data_name, static_cast<SynTestData*>(_pSharpnessData));

}

void Ts_BravoSharpness::get_sharpness(int height, int width, float pImg[MAXROW][MAXCOL], float* sharpnessMeasure, SharpnessData* pSharpness)
{
	int i, j;
	int m, n;
	double x, y;
	int tempSum = 0;
	int sum;
	int sharpImage[MAXROW][MAXCOL];

	int tempImg[3][3];
	int tempGx[3][3];
	int tempGy[3][3];

	int product[MAXROW][MAXCOL];

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			//==================
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempImg[m][n] = (int)pImg[i + m][j + n];
				}
			}
			//==================
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempGx[m][n] = tempImg[m][n] * m_kernelSharpnessGx[m][n];
				}
			}
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempSum += tempGx[m][n];
				}
			}
			x = tempSum;
			tempSum = 0;
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempGy[m][n] = tempImg[m][n] * m_kernelSharpnessGy[m][n];
				}
			}
			for (m = 0; m<3; m++)
			{
				for (n = 0; n<3; n++)
				{
					tempSum += tempGy[m][n];
				}
			}
			y = tempSum;
			tempSum = 0;
			sum = (int)((x*x) + (y*y));
			sum = (int)sqrt((long double)sum);
			sharpImage[i][j] = sum;
			sum = 0;
		}
	}

	gradient(height, width, sharpImage, pSharpness->Gx, pSharpness->Gy, pSharpness);
	for (i = 0; i<height - 2; i++)
	{
		for (j = 0; j<width - 2; j++)
		{
			product[i][j] = (pSharpness->Gx[i][j] * pSharpness->Gx[i][j]) + (pSharpness->Gy[i][j] * pSharpness->Gy[i][j]);
			product[i][j] = (int)sqrt((long double)product[i][j]);
		}
	}
	sum = 0;
	for (i = 0; i<height - 3; i++)
	{
		for (j = 0; j<width - 4; j++)
		{
			sum += product[i][j];
		}
	}
	//overall zone
	*sharpnessMeasure = (float)(sum / ((height - 2)*(width - 4)));
}

int Ts_BravoSharpness::find_min(float* pArray, int size)
{
	int min, i, loc = 0;
	min = (int)pArray[0];
	for (i = 1; i<size; i++)
	{
		if (pArray[i] < min)
		{
			min = (int)pArray[i];
			loc = i;
		}
	}

	return loc;
}
//returns location of max
int Ts_BravoSharpness::find_max(float* pArray, int size)
{
	int max, i, loc = 0;
	max = (int)pArray[0];
	for (i = 1; i<size; i++)
	{
		if (pArray[i] > max)
		{
			max = (int)pArray[i];
			loc = i;
		}
	}

	return loc;
}

void Ts_BravoSharpness::gradient(int numRowsImg, int numColsImg, int pImg[MAXROW][MAXCOL], int Gx[MAXROW][MAXCOL], int Gy[MAXROW][MAXCOL], SharpnessData* pSharpness)
{
	int n, p;
	int k, ndim, i;
	int row, col;
	ndim = 2;

	for (k = 0; k<ndim; k++)
	{
		//56x144
		if (k == 0)
		{
			n = numRowsImg;
			p = numColsImg;
			for (i = 0; i<numRowsImg; i++)
				pSharpness->h[i] = i;
			//Take forward differences on left and right edges
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[0][i] = pImg[1][i] - pImg[0][i];
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[n][i] = pImg[n][i] - pImg[n - 1][i];

			for (row = 2; row<n; row++)
			{
				for (col = 0; col<p; col++)
				{
					pSharpness->g[row - 1][col] = (pImg[row][col] - pImg[row - 2][col]) / 2;
				}
			}
			//Obtain Gy
			for (row = 0; row<numRowsImg - 2; row++)
			{
				for (col = 0; col<numColsImg; col++)
				{
					Gy[row][col] = pSharpness->g[row][col];
				}
			}

			//init. g[][] to zero
			for (row = 0; row<MAXROW; row++)
			{
				for (col = 0; col<MAXCOL; col++)
				{
					pSharpness->g[row][col] = 0;
				}
			}
		}
		//144x56
		else
		{
			n = numColsImg;
			p = numRowsImg;
			//transpose image 
			for (row = 0; row<numColsImg; row++)
			{
				for (col = 0; col<numRowsImg; col++)
				{
					pSharpness->pImg_t[row][col] = pImg[col][row];
				}
			}
			for (i = 0; i<numColsImg; i++)
				pSharpness->h[i] = i;
			//Take forward differences on left and right edges
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[0][i] = pSharpness->pImg_t[1][i] - pSharpness->pImg_t[0][i];
			for (i = 0; i<numColsImg; i++)
				pSharpness->g[n][i] = pSharpness->pImg_t[n][i] - pSharpness->pImg_t[n - 1][i];

			for (row = 2; row<n; row++)
			{
				for (col = 0; col<p; col++)
				{
					pSharpness->g[row - 1][col] = (pSharpness->pImg_t[row][col] - pSharpness->pImg_t[row - 2][col]) / 2;
				}
			}
			//transpose image and Obtain Gx			
			for (row = 0; row<p; row++)
			{
				for (col = 0; col<n; col++)
				{
					Gx[row][col] = pSharpness->g[col][row];
				}
			}
		}
	}
}
