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

signals:

protected:

	void run();

private:

	Syn_TestEngine *_pTestEngine;
	
	bool _stopped;
};

#endif // SYNTHREAD_H
