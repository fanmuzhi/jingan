#pragma once

#include "Syn_Test_Utils.h"
#include "Syn_Config_Utils_MT.h"

class Syn_Dut_Utils
{
public:
	Syn_Dut_Utils();
	~Syn_Dut_Utils();


	Syn_Config_MT_Info	 Config_MT_Info;

	vector<SynTestData*> list_TestData;
};

