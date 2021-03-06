#include "Syn_TestStepFactory.h"
#include "Syn_BravoFingerprintTest.h"
#include "Ts_BravoInitializationStep.h"
#include "Ts_BravoProgrammingMF.h"
#include "Ts_BravoProgrammingIOTA.h"
#include "Ts_BravoDRdyTest.h"
#include "Ts_BravoSleepCurrentTest.h"
#include "Ts_BravoCalibrate.h"
#include "Ts_BravoAcqImgNoFinger.h"
#include "Ts_BravoWaitStimulus.h"
#include "Ts_BravoAcqImgFinger.h"
#include "Ts_BravoWOF_Baseline.h"
#include "Ts_BravoWOF_Signal.h"
#include "Ts_BravoCurrentTest.h"
#include "Ts_BravoSNRTest.h"
#include "Ts_BravoImperfections.h"
#include "Ts_BravoSharpness.h"
#include "Ts_BravoIOTACheck.h"
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
	else if (string("ProgrammingIOTA") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoProgrammingIOTA(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("DRdyTest") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoDRdyTest(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("SleepCurrentTest") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoSleepCurrentTest(strTestStepName, pBravoModule, pDutUtils);
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
	else if (string("WOF_Baseline") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoWOF_Baseline(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("WOF_Signal") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoWOF_Signal(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("SNRTest") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoSNRTest(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("CurrentTest") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoCurrentTest(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("SharpnessTest") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoSharpness(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("Imperfections") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoImperfections(strTestStepName, pBravoModule, pDutUtils);
	}
	else if (string("IOTACheck") == strTestStepName)
	{
		opBravoTestStep = new Ts_BravoIOTACheck(strTestStepName, pBravoModule, pDutUtils);
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