#include "Syn_Dut_Utils.h"


Syn_Dut_Utils::Syn_Dut_Utils()
:_pDutTestResult(NULL)
{
	_pDutTestResult = new dut_test_result();
}

Syn_Dut_Utils::~Syn_Dut_Utils()
{
	if (NULL != _pDutTestResult)
	{
		delete _pDutTestResult;
		_pDutTestResult = NULL;
	}
}
