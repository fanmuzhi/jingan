#pragma once

#include "Syn_Test_Utils.h"
#include "Syn_Config_Utils_MT.h"

#include <map>

typedef struct dut_test_result_t
{
	string strSensorSerialNumber;

	vector<string> list_bincodes;
	map<string, string>	map_teststep_ispass;

	//map<TestData_Type_t, SynTestData*> map_Testdata;
	vector<SynTestData*> list_testdata;
}dut_test_result;

class Syn_Dut_Utils
{
public:

	Syn_Dut_Utils();
	~Syn_Dut_Utils();

	Syn_Config_MT_Info Config_MT_Info;

	dut_test_result *_pDutTestResult;
};

