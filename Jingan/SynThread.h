#ifndef SYNTHREAD_H
#define SYNTHREAD_H

#include <QThread>

#include "Syn_TestEngine.h"

enum FlagType{Init, Final, All};

class SynThread : public QThread
{
	Q_OBJECT

public:
	SynThread();
	~SynThread();

	void SetTestEngine(Syn_TestEngine *ipTestEngine);

	void SetStopTag(bool stopTag);

	void SetFlagType(FlagType iFlagType);

signals:

	void sendTestStep(unsigned int EngineNumber, const QString strTestStep, const QString strPassOrFail);

	void sendTestData(unsigned int EngineNumber, const dut_test_result *pTestData);

protected:

	void run();

private:

	Syn_TestEngine *_pTestEngine;
	
	bool _stopped;

	FlagType _flagType;
};

#endif // SYNTHREAD_H
