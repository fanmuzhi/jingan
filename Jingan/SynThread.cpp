#include "SynThread.h"

SynThread::SynThread()
: QThread()
, _pTestEngine(NULL)
, _stopped(true)
{
}

SynThread::~SynThread()
{
	_ListOfTestStep.clear();
}

void SynThread::SetTestEngine(Syn_TestEngine *ipTestEngine)
{
	if (NULL != ipTestEngine)
		_pTestEngine = ipTestEngine;
}

void SynThread::SetStopTag(bool stopTag)
{
	_stopped = stopTag;
}

void SynThread::SetTestStep(vector<string> iListOfTestStep)
{
	if (0 != iListOfTestStep.size())
	{
		_ListOfTestStep.clear();
		for (size_t i = 0; i < iListOfTestStep.size(); i++)
			_ListOfTestStep.push_back(iListOfTestStep[i]);
	}
}

void SynThread::run()
{
	uint32_t rc = 0;

	if (NULL == _pTestEngine)
		return;

	Syn_TestEngine::EngineState EngineStatus = _pTestEngine->GetStatus();
	if (Syn_TestEngine::error == EngineStatus)
		return;

	unsigned int TestStepCounts = _ListOfTestStep.size();
	if (0 == TestStepCounts)
		return;

	uint32_t EngineNumber(0);
	_pTestEngine->GetTestEngineNumber(EngineNumber);

	//WaitStimulus
	if (1 == TestStepCounts && "WaitStimulus" == _ListOfTestStep[0])
	{
		_pTestEngine->ExecuteTestStep(_ListOfTestStep[0], Syn_TestEngine::Setup);
		while (!_stopped)
		{
			dut_test_result *pTestData = NULL;
			rc = _pTestEngine->ExecuteTestStep(_ListOfTestStep[0], Syn_TestEngine::Excute);
			_pTestEngine->GetTestData(pTestData);
			emit sendTestData(EngineNumber, pTestData);
		}
		_pTestEngine->ExecuteTestStep(_ListOfTestStep[0], Syn_TestEngine::Cleanup);
	}
	else
	{
		dut_test_result *pTestData = NULL;
		for (size_t i = 0; i < TestStepCounts; i++)
		{
			rc = _pTestEngine->ExecuteTestStep(_ListOfTestStep[i]);
			_pTestEngine->GetTestData(pTestData);
			if (0 != rc || NULL == pTestData)
			{
				QString strErrorCode = QString::number(rc, 16);
				emit sendTestStep(EngineNumber, QString::fromStdString(_ListOfTestStep[i]), "Fail(0x" + strErrorCode + ")");
				emit sendTestData(EngineNumber, pTestData);
				return;
			}
			else
			{
				//sensor serialnumber
				if ("InitializationStep" == _ListOfTestStep[i])
				{
					QString strPassFail_SensorSN = QString::fromStdString(pTestData->map_teststep_ispass[_ListOfTestStep[i]]) + "_" + QString::fromStdString(pTestData->strSensorSerialNumber);
					emit sendTestStep(EngineNumber, QString::fromStdString(_ListOfTestStep[i]), strPassFail_SensorSN);
				}
				else
				{
					emit sendTestStep(EngineNumber, QString::fromStdString(_ListOfTestStep[i]), QString::fromStdString(pTestData->map_teststep_ispass[_ListOfTestStep[i]]));
				}
			}
		}

		emit sendTestData(EngineNumber, pTestData);
	}
}