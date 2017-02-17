#ifndef SYNIMAGINGTHREAD_H
#define SYNIMAGINGTHREAD_H

//local
#include "Syn_TestEngine.h"

//Qt
#include <QThread>


class SynImagingThread : public QThread
{
	Q_OBJECT

public:
	SynImagingThread() :_stopped(true){};
	~SynImagingThread(){};

	void SetTestEngine(Syn_TestEngine *ipTestEngine)
	{
		if (NULL != ipTestEngine)
			_pTestEngine = ipTestEngine;
	}

	void SetStopTag(bool stoptag)
	{
		_stopped = stoptag;
	}

signals:

	void sendImage(unsigned int EngineNumber, const dut_test_result *pTestData);

protected:

	void run()
	{
		uint32_t rc(0);
		if (NULL == _pTestEngine)
			return;

		unsigned int EngineNumber(0);
		_pTestEngine->GetTestEngineNumber(EngineNumber);

		vector<string> listOfTestStepName;
		_pTestEngine->GetTestStepList(listOfTestStepName);
		if (0 == listOfTestStepName.size())
			return;

		unsigned int iWaitStimulusPosition(0);
		bool find(false);
		for (size_t i = 0; i < listOfTestStepName.size(); i++)
		{
			if (std::string("WaitStimulus") == listOfTestStepName[i])
			{
				find = true;
				iWaitStimulusPosition = i;
				break;
			}
		}
		if (!find)
			return;

		dut_test_result *pTestData = NULL;
		_pTestEngine->ExecuteTestStep(listOfTestStepName[iWaitStimulusPosition], Syn_TestEngine::Setup);
		_pTestEngine->GetTestData(pTestData);
		while (!_stopped)
		{
			Syn_TestEngine::EngineState Status = _pTestEngine->GetStatus();
			if (Syn_TestEngine::error != Status)
			{
				rc = _pTestEngine->ExecuteTestStep(listOfTestStepName[iWaitStimulusPosition], Syn_TestEngine::Excute);
				//rc = _pTestEngine->ExecuteTestStep(listOfTestStepName[iWaitStimulusPosition]);
				_pTestEngine->GetTestData(pTestData);
				emit sendImage(EngineNumber, pTestData);
			}
		}
		_pTestEngine->ExecuteTestStep(listOfTestStepName[iWaitStimulusPosition], Syn_TestEngine::Cleanup);
		_pTestEngine->GetTestData(pTestData);
	}

private:

	Syn_TestEngine *_pTestEngine;

	bool _stopped;
};

#endif // SYNIMAGINGTHREAD_H