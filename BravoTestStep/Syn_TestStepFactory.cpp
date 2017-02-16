#include "Syn_TestStepFactory.h"
#include "Syn_BravoFingerprintTest.h"
#include "Ts_BravoInitializationStep.h"
#include "Ts_BravoProgrammingMF.h"
#include "Ts_BravoProgrammingIOTA.h"
#include "Ts_BravoCalibrate.h"
#include "Ts_BravoAcqImgNoFinger.h"
#include "Ts_BravoWaitStimulus.h"
#include "Ts_BravoAcqImgFinger.h"
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
	else if (string("ProgrammingMissionFirmware") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoProgrammingMF(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (std::string::npos != strTestStepName.find("ProgrammingIOTA"))
	{
		opBravoTestStep = new Ts_BravoProgrammingIOTA(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("Calibrate") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoCalibrate(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("AcqImgNoFinger") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoAcqImgNoFinger(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("WaitStimulus") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoWaitStimulus(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("AcqImgFinger") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoAcqImgFinger(strTestStepName, pBravoModule, pDutUtils);
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