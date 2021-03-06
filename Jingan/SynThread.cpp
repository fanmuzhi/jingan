#include "SynThread.h"

SynThread::SynThread()
: QThread()
, _pTestEngine(NULL)
, _stopped(true)
{
}

SynThread::~SynThread()
{
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

void SynThread::SetFlagType(FlagType iFlagType)
{
	_flagType = iFlagType;
}

void SynThread::run()
{
	uint32_t rc = 0;

	if (NULL == _pTestEngine)
		return;

	Syn_TestEngine::EngineState EngineStatus = _pTestEngine->GetStatus();
	if (Syn_TestEngine::error == EngineStatus)
		return;

	vector<string> ListOfTestStep;
	_pTestEngine->GetTestStepList(ListOfTestStep);

	unsigned int TestStepCounts = ListOfTestStep.size();
	if (0 == TestStepCounts)
		return;

	unsigned int iWaitStimulusPosition(0);
	for (size_t i = 0; i < ListOfTestStep.size(); i++)
	{
		if (string("WaitStimulus") == ListOfTestStep[i])
		{
			iWaitStimulusPosition = i;
			break;
		}
	}

	uint32_t EngineNumber(0);
	_pTestEngine->GetTestEngineNumber(EngineNumber);

	dut_test_result *pTestData = NULL;
	if (Init == _flagType)
	{
		rc = _pTestEngine->Open();
		if (rc == 0)
		{
			for (size_t t = 0; t < iWaitStimulusPosition; t++)
			{
				rc = _pTestEngine->ExecuteTestStep(ListOfTestStep[t]);
				_pTestEngine->GetTestData(pTestData);
				if (0 != rc || NULL == pTestData)
				{
					QString strErrorCode = QString::number(rc, 16);
					emit sendTestStep(EngineNumber, QString::fromStdString(ListOfTestStep[t]), "Fail(0x" + strErrorCode + ")");
					emit sendTestData(EngineNumber, pTestData);
					return;
				}
				else
				{
					QString strPassOrFail = QString::fromStdString(pTestData->map_teststep_ispass[ListOfTestStep[t]]);
					if ("InitializationStep" == ListOfTestStep[t])
					{
						strPassOrFail = strPassOrFail + "_" + QString::fromStdString(pTestData->strSensorSerialNumber);
					}
					emit sendTestStep(EngineNumber, QString::fromStdString(ListOfTestStep[t]), strPassOrFail);
				}
			}
			emit sendTestData(EngineNumber, pTestData);
		}
	}
	else if (Final == _flagType)
	{
		for (size_t t = iWaitStimulusPosition + 1; t < ListOfTestStep.size(); t++)
		{
			rc = _pTestEngine->ExecuteTestStep(ListOfTestStep[t]);
			_pTestEngine->GetTestData(pTestData);
			if (0 != rc || NULL == pTestData)
			{
				QString strErrorCode = QString::number(rc, 16);
				emit sendTestStep(EngineNumber, QString::fromStdString(ListOfTestStep[t]), "Fail(0x" + strErrorCode + ")");
				emit sendTestData(EngineNumber, pTestData);
				return;
			}
			else
			{
				emit sendTestStep(EngineNumber, QString::fromStdString(ListOfTestStep[t]), QString::fromStdString(pTestData->map_teststep_ispass[ListOfTestStep[t]]));
			}
		}
		emit sendTestData(EngineNumber, pTestData);
	}
	else
	{
		rc = _pTestEngine->Open();
		if (rc == 0)
		{
			dut_test_result *pTestData = NULL;
			for (size_t t = 0; t < TestStepCounts; t++)
			{
				rc = _pTestEngine->ExecuteTestStep(ListOfTestStep[t]);
				_pTestEngine->GetTestData(pTestData);
				if (0 != rc || NULL == pTestData)
				{
					QString strErrorCode = QString::number(rc, 16);
					emit sendTestStep(EngineNumber, QString::fromStdString(ListOfTestStep[t]), "Fail(0x" + strErrorCode + ")");
					emit sendTestData(EngineNumber, pTestData);
					return;
				}
				else
				{
					string sPassOrFail = pTestData->map_teststep_ispass[ListOfTestStep[t]];
					QString strPassOrFail = QString::fromStdString(sPassOrFail);
					if ("InitializationStep" == ListOfTestStep[t])
					{
						strPassOrFail = strPassOrFail + "_" + QString::fromStdString(pTestData->strSensorSerialNumber);
					}
					emit sendTestStep(EngineNumber, QString::fromStdString(ListOfTestStep[t]), strPassOrFail);
				}
			}

			emit sendTestData(EngineNumber, pTestData);
		}
	}
}