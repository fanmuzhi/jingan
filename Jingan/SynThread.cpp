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

void SynThread::run()
{
	if (NULL == _pTestEngine)
		return;
}