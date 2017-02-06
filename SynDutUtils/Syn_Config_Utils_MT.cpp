#include "Syn_Config_Utils_MT.h"

//std
#include <iostream>
#include <fstream>
#include <io.h>

/*
	Syn_Config_Utils_MT
*/

Syn_Config_Utils_MT::Syn_Config_Utils_MT(string strConfigFilePath)
:_rapidxmlfile(strConfigFilePath.c_str())
, _rapidxmldoc()
, _rapidxmlrootNode(NULL)
{
}

Syn_Config_Utils_MT::~Syn_Config_Utils_MT()
{
}

uint32_t Syn_Config_Utils_MT::CreateConfigUtils_MT(string strConfigFilePath, Syn_Config_Utils_MT * &opConfigUtils_MT)
{
	opConfigUtils_MT = NULL;

	if (-1 == _access(strConfigFilePath.c_str(), 4))
	{
		//LOG(ERROR) << "Error:Syn_SysConfigOperation::GetSysConfigInstance() - strConfigFilePath is not exists!" << endl;
		return ERROR_CONFIG_PATH;
	}

	opConfigUtils_MT = new Syn_Config_Utils_MT(strConfigFilePath);

	//reparse
	bool rc = opConfigUtils_MT->parse();
	if (!rc)
	{
		delete opConfigUtils_MT;
		opConfigUtils_MT = NULL;
		//LOG(ERROR) << "Error:Syn_SysConfigOperation::GetSysConfigInstance() - ::Parse is failed!" << endl;
		return ERROR_CONFIG_PARSE;
	}

	return 0;
}

uint32_t Syn_Config_Utils_MT::GetPatchInfoFromPath(string strPathFilePath, Syn_Patch_Info &oPatchInfo)
{
	if (-1 == _access(strPathFilePath.c_str(), 4))
	{
		return ERROR_CONFIG_PATH;
	}

	ifstream PatchFile;

	//open file
	PatchFile.open(strPathFilePath, std::ifstream::binary);
	if (!PatchFile)
		return ERROR_CONFIG_PARSE;

	PatchFile.seekg(0, PatchFile.end);
	unsigned int length = PatchFile.tellg();
	PatchFile.seekg(0, PatchFile.beg);

	// allocate memory:
	char * buffer = new char[length];
	// read data as a block:
	PatchFile.read(buffer, length);

	PatchFile.close();

	uint8_t *arrPatch = new uint8_t[length];
	for (unsigned i = 0; i < length; i++)
	{
		arrPatch[i] = (uint8_t)(buffer[i]);
	}

	delete[] buffer;
	buffer = NULL;

	oPatchInfo.patchbuflen = length;
	oPatchInfo.patchbufp = arrPatch;
	oPatchInfo.strPatchFileName = strPathFilePath;
	oPatchInfo.strPatchName = strPathFilePath;

	return 0;
}

bool Syn_Config_Utils_MT::parse()
{
	_rapidxmldoc.parse<0>(_rapidxmlfile.data());

	//root
	_rapidxmlrootNode = NULL;
	_rapidxmlrootNode = _rapidxmldoc.first_node();
	if (NULL == _rapidxmlrootNode)
	{
		//LOG(ERROR) << "Error:Syn_SysConfigOperation::Parse() - _rapidxmlrootNode is NULL!" << endl;
		return false;
	}

	return true;
}

void Syn_Config_Utils_MT::convertAsciiToBinary(const string& strAsciiValue, uint8_t *arrBuf, uint32_t size)
{
	char *temp;

	unsigned int nNumBytes = strAsciiValue.length() / 2;
	//Translate the ASCII into binary values.
	for (unsigned int i = 0; (i<nNumBytes) && (i<size); i++)
	{
		std::string strTemp(strAsciiValue, i * 2, 2);

		uint8_t nVal = (uint8_t)strtol(strTemp.c_str(), &temp, 16);
		arrBuf[i] = nVal;
	}
}

uint32_t Syn_Config_Utils_MT::GetConfigMTInfo(Syn_Config_MT_Info &oConfiguration)
{
	bool rc(true);

	if (NULL == _rapidxmlrootNode)
	{
		return ERROR_CONFIG_ERR;
	}

	for (rapidxml::xml_node<char> *node = _rapidxmlrootNode->first_node(); node != NULL; node = node->next_sibling())
	{
		std::string strNodeName = node->name();
		if (std::string("AutoController") == strNodeName)
		{
			oConfiguration.strAutoController = node->value();
		}
		else if (std::string("DutType") == strNodeName)
		{
			oConfiguration.strDutType = node->value();
		}
		else if (std::string("DutController") == strNodeName)
		{
			oConfiguration.strDutController = node->value();
		}
		else if (std::string("DutCom") == strNodeName)
		{
			oConfiguration.strDutCom = node->value();
		}
		else if (std::string("DutPwrVdd_mV") == strNodeName)
		{
			oConfiguration.vdd_mv = atoi(node->value());
		}
		else if (std::string("DutPwrVio_mV") == strNodeName)
		{
			oConfiguration.vio_mv = atoi(node->value());
		}
		else if (std::string("DutPwrVled_mV") == strNodeName)
		{
			oConfiguration.vled_mv = atoi(node->value());
		}
		else if (std::string("DutPwrVddh_mV") == strNodeName)
		{
			oConfiguration.vddh_mv = atoi(node->value());
		}
		else if (std::string("NumRows") == strNodeName)
		{
			oConfiguration.rowNumber = atoi(node->value());		
		}
		else if (std::string("NumCols") == strNodeName)
		{
			oConfiguration.columnNumber = atoi(node->value());
		}
		else if (std::string("TestSeq") == strNodeName)
		{
			//TestStep
			for (rapidxml::xml_node<char> *TestNode = node->first_node(); TestNode != NULL; TestNode = TestNode->next_sibling())
			{
				Syn_TestStep_Info curTestStepInfo;
				curTestStepInfo.strNodeName = TestNode->name();
				curTestStepInfo.strTestStepName = TestNode->value();

				rapidxml::xml_attribute<> *pAttribute = TestNode->first_attribute("Args");
				if (NULL != pAttribute)
				{
					curTestStepInfo.strTestStepArgs = pAttribute->value();
				}

				oConfiguration.list_TestStep_Info.push_back(curTestStepInfo);
			}
		}
		else
		{
			//Patch
			Syn_Patch_Info curPatchInfo;
			std::string strPatchName(strNodeName);

			rapidxml::xml_attribute<> *attribute = node->first_attribute("Args");
			if (NULL != attribute)
			{
				curPatchInfo.strPatchFileName = attribute->value();
			}

			std::string strPatchTextValue(node->value());
			int iTextLength = strPatchTextValue.length();
			if (0 == iTextLength)
			{
				curPatchInfo.strPatchName = strPatchName;
				curPatchInfo.patchbufp = NULL;
				curPatchInfo.patchbuflen = 0;
			}
			else
			{
				uint8_t *arrPatch = new uint8_t[iTextLength / 2];
				convertAsciiToBinary(strPatchTextValue, arrPatch, iTextLength / 2);

				curPatchInfo.strPatchName = strPatchName;
				curPatchInfo.patchbufp = arrPatch;
				curPatchInfo.patchbuflen = iTextLength / 2;
			}

			oConfiguration.list_Patch_Info.push_back(curPatchInfo);
		}
	}

	return 0;
}