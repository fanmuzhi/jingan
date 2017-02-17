#pragma once

#include "FpBravoModule.h"

#include "Syn_Test_Utils.h"
#include "Syn_Dut_Utils.h"
#include "Syn_TestStep.h"

class Syn_TestStepFactory
{
public:

	//Syn_TestStepFactory();
	~Syn_TestStepFactory();

	static uint32_t CreateBravoTestStep(string strTestStepName, string strTestArgs, FpBravoModule * &pBravoModule, Syn_Dut_Utils * &pDutUtils, Syn_TestStep * &opBravoTestStep);

private:

	Syn_TestStepFactory();
};

