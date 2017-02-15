#include "Syn_TestStepFactory.h"
#include "Syn_BravoFingerprintTest.h"
#include "Ts_BravoInitializationStep.h"
#include "Ts_BravoCalibrate.h"
#include "Ts_BravoWaitStimulus.h"
#include "Ts_BravoFinalizationStep.h"

Syn_TestStepFactory::Syn_TestStepFactory()
{
}

Syn_TestStepFactory::~Syn_TestStepFactory()
{
}

uint32_t Syn_TestStepFactory::CreateBravoTestStep(string strTestStepName, string strTestArgs, FpBravoModule * &pBravoModule, Syn_Dut_Utils * &pDutUtils, Syn_TestStep * &opBravoTestStep)
{
	uint32_t rc = 0;
	opBravoTestStep = NULL;

	if (NULL == pDutUtils)
	{
		return ERROR_DUTUTILS_NULL;
	}
	if (NULL == pBravoModule)
	{
		return ERROR_BRAVOMODULE_NULL;
	}

	if (string("InitializationStep") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoInitializationStep(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("Calibrate") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoCalibrate(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("WaitStimulus") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoWaitStimulus(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("FinalizationStep") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoFinalizationStep(strTestStepName, pBravoModule, pDutUtils);
	}
	else
	{
		rc = ERROR_TESTSTEP_UNDEFINE;
	}

	return rc;
}