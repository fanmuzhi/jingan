//Local
#include "Syn_Site.h"

#include "Synaptics_Site.h"

Synaptics_Site::Synaptics_Site(uint8_t siteNumber, std::string deviceSerNumber, std::string strConfigFilePath, const SynapticsAdcBaseLineInfo &adcInfo)
:_pSyn_Site(NULL)
, _pSynaptics_TestResult(NULL)
{
	AdcBaseLineInfo adcBaseLineInfo;

	adcBaseLineInfo.m_bExecuted = 1;
	adcBaseLineInfo.m_nVdd = adcInfo.m_nVdd;
	adcBaseLineInfo.m_nVddh = adcInfo.m_nVddh;
	adcBaseLineInfo.m_nVio = adcInfo.m_nVio;
	adcBaseLineInfo.m_nVled = adcInfo.m_nVled;

	for (int i = 0; i < NUM_CURRENT_VALUES; i++)
	{
		adcBaseLineInfo.m_arrAdcBaseLines[i] = adcInfo.arrAdcBaseLines[i];
	}

	std::string path;
	uint32_t rc = Syn_Site::CreateSiteInstance(siteNumber, deviceSerNumber, strConfigFilePath, adcBaseLineInfo, _pSyn_Site);
	if (rc != 0 || NULL == _pSyn_Site)
	{
		throw exception("Construct site failed.");
	}
}

Synaptics_Site::~Synaptics_Site()
{
	if (NULL != _pSyn_Site)
	{
		delete _pSyn_Site;
		_pSyn_Site = NULL;
	}

	if (NULL != _pSynaptics_TestResult)
	{
		delete _pSynaptics_TestResult;
		_pSynaptics_TestResult = NULL;
	}
}

uint32_t Synaptics_Site::Open()
{
	if (NULL == _pSyn_Site)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		if (NULL == _pSynaptics_TestResult)
			_pSynaptics_TestResult = new Synaptics_TestResult();
		return _pSyn_Site->Open();
	}
}

std::vector<std::string> Synaptics_Site::GetTestStepList()
{
	vector<string> testSteps;
	testSteps.clear();
	
	if (NULL == _pSyn_Site)
	{
		return testSteps;
	}

	_pSyn_Site->GetTestStepList(testSteps);
	
	return testSteps;
}

uint32_t Synaptics_Site::ExecuteTestStep(std::string strTestStepName)
{
	if (NULL == _pSyn_Site)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		return _pSyn_Site->ExecuteTestStep(strTestStepName);
	}
}

uint32_t Synaptics_Site::GetTestResult(Synaptics_TestResult * &opSynapticsTestResult)
{
	opSynapticsTestResult = NULL;

	if (NULL == _pSyn_Site)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}

	Syn_DutTestResult* pTestResult = NULL;
	uint32_t rc = _pSyn_Site->GetTestResult(pTestResult);
	if (rc != 0 || NULL == pTestResult)
	{
		return rc;
	}
	Syn_DutTestInfo *pTestInfo = NULL;
	rc = _pSyn_Site->GetTestInfo(pTestInfo);
	if (rc != 0)
	{
		return rc;
	}

	if (NULL == _pSynaptics_TestResult)
		_pSynaptics_TestResult = new Synaptics_TestResult();

	//serial number
	_pSynaptics_TestResult->strSensorNumber = pTestResult->_versionResult.sSerialNumber;

	//each test step and test result
	_pSynaptics_TestResult->mapStepResult.clear();
	for (std::map<std::string, std::string>::iterator i = pTestResult->_mapTestPassInfo.begin(); i != pTestResult->_mapTestPassInfo.end(); i++)
	{
		_pSynaptics_TestResult->mapStepResult.insert(std::map<std::string, std::string>::value_type(i->first, i->second));
	}

	//bin codes
	_pSynaptics_TestResult->listOfBincodes.clear();
	for (size_t i = 0; i < pTestResult->_binCodes.size(); i++)
	{
		_pSynaptics_TestResult->listOfBincodes.push_back(pTestResult->_binCodes[i]);
	}

	//snr
	_pSynaptics_TestResult->snrValue = pTestResult->_snrResults.SNR[6];
	_pSynaptics_TestResult->huaweiSnrValue = 0;
	if (NULL != pTestInfo)
	{
		if (pTestInfo->_huaweiIqTestInfo._bExecuted)
		{
			_pSynaptics_TestResult->huaweiSnrValue = pTestResult->_huaweiIqTestResults.snr;
		}
	}

	//no finger image
	if (pTestInfo->_acqImgNoFingerInfo.m_bExecuted && !(pTestInfo->_acqImgFingerInfo.m_bExecuted))
	{
		for (auto i = 0; i < pTestResult->_acqImgNoFingerResult.iRealRowNumber; i++)
		{
			for (auto j = 0; j < pTestResult->_acqImgNoFingerResult.iRealColumnNumber; j++)
			{
				_pSynaptics_TestResult->arrImageNoFinger[i][j] = pTestResult->_acqImgNoFingerResult.arr_ImageFPSFrame.arr[i][j];
			}
		}
		_pSynaptics_TestResult->ImageNoFingerRow = pTestResult->_acqImgNoFingerResult.iRealRowNumber;
		_pSynaptics_TestResult->ImageNoFingerCol = pTestResult->_acqImgNoFingerResult.iRealColumnNumber;
	}
	

	//finger image
	if (pTestInfo->_snrInfo.m_bExecuted)
	{
		for (auto i = 0; i < pTestResult->_acqImgFingerResult.iRealRowNumber; i++)
		{
			for (auto j = 0; j < pTestResult->_acqImgFingerResult.iRealColumnNumber; j++)
			{
				_pSynaptics_TestResult->arrImageFinger[i][j] = pTestResult->_acqImgFingerResult.arr_ImageFPSFrame.arr[i][j];
			}
		}
		_pSynaptics_TestResult->ImageFingerRow = pTestResult->_acqImgFingerResult.iRealRowNumber;
		_pSynaptics_TestResult->ImageFingerCol = pTestResult->_acqImgFingerResult.iRealColumnNumber;
	}

	opSynapticsTestResult = _pSynaptics_TestResult;
	
	return rc;
}

void Synaptics_Site::WriteLog(std::string strFolderPath, std::string strFileName)
{
	if (NULL != _pSyn_Site)
	{
		_pSyn_Site->WriteLog(strFolderPath, strFileName);
	}
}

uint32_t Synaptics_Site::Close()
{
	if (NULL == _pSyn_Site)
	{
		return Syn_ExceptionCode::Syn_UnknownError;
	}
	else
	{
		if (NULL != _pSynaptics_TestResult)
		{
			delete _pSynaptics_TestResult;
			_pSynaptics_TestResult = NULL;
		}

		return _pSyn_Site->Close();
	}
}