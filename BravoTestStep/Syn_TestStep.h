#pragma once

//Local
#include "FpBravoModule.h"
#include "syn_bridge.h"

//DutUtls
#include "Syn_Test_Utils.h"
#include "Syn_Dut_Utils.h"

class Syn_TestStep
{
public:

	Syn_TestStep(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
		:_strName(strName), _pSynModule(pSynModule), _pSynDutUtils(pSynDutUtils)
	{
	}
	
	virtual ~Syn_TestStep()
	{
	}

	string GetName()
	{
		return _strName;
	}

	virtual	void SetUp() = 0;

	virtual void Execute() = 0;

	virtual void ProcessData() = 0;

	virtual void CleanUp() = 0;

protected:

	string _strName;

	FpBravoModule *_pSynModule;
	//syn_bridge	  *_pSynBridge;

	Syn_Dut_Utils *_pSynDutUtils;
};

