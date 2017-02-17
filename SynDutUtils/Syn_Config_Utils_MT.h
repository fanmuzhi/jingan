#pragma once

//rapidxml
#include "rapidxml.hpp"  
#include "rapidxml_utils.hpp" 

//std C
#include "stdint.h"

//std
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define ERROR_CONFIG_ERR	0x1600
#define ERROR_CONFIG_PATH	0x1601
#define ERROR_CONFIG_PARSE	0x1602
#define ERROR_CONFIG_COUNTS	0x1603

typedef struct Syn_TestStep_Info_t
{
	string strNodeName;

	string strTestStepName;
	string strTestStepArgs;
}Syn_TestStep_Info;

typedef struct Syn_Patch_Info_t
{
	string    strPatchName;
	string    strPatchFileName;

	uint8_t   *patchbufp;
	uint32_t  patchbuflen;
}Syn_Patch_Info;

/*
	config_mt(xml) structure
*/

struct Syn_Config_MT_Info
{
	Syn_Config_MT_Info(){}
	~Syn_Config_MT_Info()
	{
		for (size_t i = list_Patch_Info.size(); i >= 1; i--)
		{
			Syn_Patch_Info Cur_Patch_Info = list_Patch_Info[i - 1];
			if (NULL != Cur_Patch_Info.patchbufp && 0 != Cur_Patch_Info.patchbuflen)
			{
				delete[] Cur_Patch_Info.patchbufp;
				Cur_Patch_Info.patchbufp = NULL;
			}
		}
		list_Patch_Info.clear();
	}

	const Syn_Config_MT_Info& Syn_Config_MT_Info::operator=(Syn_Config_MT_Info& original)
	{
		strAutoController = original.strAutoController;
		strDutType = original.strDutType;
		strDutController = original.strDutController;
		strDutCom = original.strDutCom;

		vdd_mv = original.vdd_mv;
		vio_mv = original.vio_mv;
		vled_mv = original.vled_mv;
		vddh_mv = original.vddh_mv;

		rowNumber = original.rowNumber;
		columnNumber = original.columnNumber;

		list_TestStep_Info.clear();
		for (size_t i = 1; i <= original.list_TestStep_Info.size(); i++)
		{
			Syn_TestStep_Info curTestStepInfo;
			curTestStepInfo.strNodeName = (original.list_TestStep_Info)[i - 1].strNodeName;
			curTestStepInfo.strTestStepName = (original.list_TestStep_Info)[i - 1].strTestStepName;
			curTestStepInfo.strTestStepArgs = (original.list_TestStep_Info)[i - 1].strTestStepArgs;
			list_TestStep_Info.push_back(curTestStepInfo);
		}

		list_Patch_Info.clear();
		for (size_t i = 1; i <= original.list_Patch_Info.size(); i++)
		{
			Syn_Patch_Info curPatchInfo;
			curPatchInfo.strPatchFileName = (original.list_Patch_Info)[i - 1].strPatchFileName;
			curPatchInfo.strPatchName = (original.list_Patch_Info)[i - 1].strPatchName;
			curPatchInfo.patchbuflen = (original.list_Patch_Info)[i - 1].patchbuflen;

			curPatchInfo.patchbufp = new uint8_t[curPatchInfo.patchbuflen];
			for (unsigned int j = 0; j < curPatchInfo.patchbuflen; j++)
				(curPatchInfo.patchbufp)[j] = ((original.list_Patch_Info)[i - 1].patchbufp)[j];

			list_Patch_Info.push_back(curPatchInfo);
		}

		return *this;
	}

	//method
	bool GetPatchInfo(const string &strPatchName, Syn_Patch_Info &oPatchInfo)
	{
		bool IsExists(false);
		for (size_t i = 1; i <= list_Patch_Info.size(); i++)
		{
			if (strPatchName == list_Patch_Info[i - 1].strPatchName)
			{
				oPatchInfo = list_Patch_Info[i - 1];
				IsExists = true;
				break;
			}
		}

		return IsExists;
	}

	bool GetTestStepInfo(const string &strTestStepName, string &oArgsValue)
	{
		bool IsExists(false);
		for (size_t i = 1; i <= list_TestStep_Info.size(); i++)
		{
			if (strTestStepName == list_TestStep_Info[i - 1].strTestStepName)
			{
				oArgsValue = list_TestStep_Info[i - 1].strTestStepArgs;
				IsExists = true;
				break;
			}
		}

		return IsExists;
	}

	//variables
	string	  strAutoController;
	string	  strDutType;
	string	  strDutController;
	string	  strDutCom;

	uint32_t  vdd_mv;
	uint32_t  vio_mv;
	uint32_t  vled_mv;
	uint32_t  vddh_mv;

	uint32_t  rowNumber;
	uint32_t  columnNumber;

	vector<Syn_Patch_Info>	  list_Patch_Info;

	vector<Syn_TestStep_Info> list_TestStep_Info;
};

class Syn_Config_Utils_MT
{
public:
	
	//Syn_Config_Utils_MT();
	~Syn_Config_Utils_MT();

public:

	static uint32_t CreateConfigUtils_MT(string strConfigFilePath, Syn_Config_Utils_MT * &opConfigUtils_MT);

	static uint32_t GetPatchInfoFromPath(string strPathFilePath, Syn_Patch_Info &oPatchInfo);

	uint32_t GetConfigMTInfo(Syn_Config_MT_Info &oConfiguration);

private:

	//constructor
	Syn_Config_Utils_MT(string strConfigFilePath);

	bool parse();

	void convertAsciiToBinary(const string& strAsciiValue, uint8_t *arrBuf, uint32_t size);

private:

	rapidxml::file<>			_rapidxmlfile;
	rapidxml::xml_document<>	_rapidxmldoc;
	rapidxml::xml_node<>		*_rapidxmlrootNode;
};

