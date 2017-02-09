#ifndef SYNTHREAD_H
#define SYNTHREAD_H

#include <QThread>

#include "Syn_TestEngine.h"

class SynThread : public QThread
{
	Q_OBJECT

public:
	SynThread();
	~SynThread();

	void SetTestEngine(Syn_TestEngine *ipTestEngine);

	void SetStopTag(bool stopTag);

	void SetTestStep(vector<string> iListOfTestStep);

signals:

	void sendTestStep(uint32_t EngineNumber, const QString strTestStep, const QString strPassOrFail);

	void sendTestData(uint32_t EngineNumber, const dut_test_result *pTestData);

protected:

	void run();

private:

	Syn_TestEngine *_pTestEngine;
	
	bool _stopped;

	vector<string> _ListOfTestStep;
};

#endif // SYNTHREAD_H
