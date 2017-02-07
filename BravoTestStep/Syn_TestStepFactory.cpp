#include "Syn_TestStepFactory.h"
#include "Syn_BravoFingerprintTest.h"
#include "Ts_BravoInitializationStep.h"

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

	if (NULL == pBravoModule)
	{
		return 1;
	}
	if (NULL == pDutUtils)
	{
		return 2;
	}

	if (std::string("InitializationStep") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoInitializationStep(strTestStepName, pBravoModule, pDutUtils);
	}
	else
	{
		rc = 3;
	}

	return rc;
}