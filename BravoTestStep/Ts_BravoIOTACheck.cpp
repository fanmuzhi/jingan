#include "Ts_BravoIOTACheck.h"
#include <bitset>
#include <stdlib.h>
#include <stdio.h>

Ts_BravoIOTACheck::Ts_BravoIOTACheck(string &strName, FpBravoModule * &pSynModule, Syn_Dut_Utils * &pSynDutUtils)
:Syn_BravoFingerprintTest(strName, pSynModule, pSynDutUtils)
, _pIOTACheckTestData(NULL)
{
}

Ts_BravoIOTACheck::~Ts_BravoIOTACheck()
{
}

void Ts_BravoIOTACheck::SetUp()
{
	Syn_Exception Exception(0);
	if (NULL == _pSynModule)
	{
		Exception.SetError(ERROR_BRAVOMODULE_NULL);
		Exception.SetDescription("BravoIOTACheck::_pSynModule is NULL!");
		throw Exception;
		return;
	}
	if (NULL == _pSynDutUtils)
	{
		Exception.SetError(ERROR_DUTUTILS_NULL);
		Exception.SetDescription("BravoIOTACheck::_pSynDutUtils is NULL!");
		throw Exception;
		return;
	}

	string strTestArgs;
	_pSynDutUtils->Config_MT_Info.GetTestStepInfo(_strName, strTestArgs);

	vector<string> listOfArgValue;
	ParseTestStepArgs(strTestArgs, listOfArgValue);
	size_t iListSize = listOfArgValue.size();
	if (iListSize < 9)
	{
		for (size_t t = 1; t <= 9 - iListSize; t++)
			listOfArgValue.push_back("");
	}
	
	_pIOTACheckTestData = new IOTACheckTestData();
	_pIOTACheckTestData->data_name = _strName;
	_pIOTACheckTestData->FrameBase_DIMS = true;
	_pIOTACheckTestData->FrameBase_FWBL = true;
	_pIOTACheckTestData->FrameBase_LNABL = true;
	_pIOTACheckTestData->FrameBase_REG16BLK = true;
	_pIOTACheckTestData->FrameBase_REG32BLK = true;
	_pIOTACheckTestData->FrameNav_LNABL = false;
	_pIOTACheckTestData->CONFIG_PSELECT = true;
	_pIOTACheckTestData->CONFIG_WOF_THRESHOLDS = true;
	_pIOTACheckTestData->CONFIG_VERSION = false;

	if (0 != listOfArgValue[0].length())
		_pIOTACheckTestData->FrameBase_DIMS = atoi(listOfArgValue[0].c_str()) ? true : false;
	if (0 != listOfArgValue[1].length())
		_pIOTACheckTestData->FrameBase_FWBL = atoi(listOfArgValue[1].c_str()) ? true : false;
	if (0 != listOfArgValue[2].length())
		_pIOTACheckTestData->FrameBase_LNABL = atoi(listOfArgValue[2].c_str()) ? true : false;
	if (0 != listOfArgValue[3].length())
		_pIOTACheckTestData->FrameBase_REG16BLK = atoi(listOfArgValue[3].c_str()) ? true : false;
	if (0 != listOfArgValue[4].length())
		_pIOTACheckTestData->FrameBase_REG32BLK = atoi(listOfArgValue[4].c_str()) ? true : false;
	if (0 != listOfArgValue[5].length())
		_pIOTACheckTestData->FrameNav_LNABL = atoi(listOfArgValue[5].c_str()) ? true : false;
	if (0 != listOfArgValue[6].length())
		_pIOTACheckTestData->CONFIG_PSELECT = atoi(listOfArgValue[6].c_str()) ? true : false;
	if (0 != listOfArgValue[7].length())
		_pIOTACheckTestData->CONFIG_WOF_THRESHOLDS = atoi(listOfArgValue[7].c_str()) ? true : false;
	if (0 != listOfArgValue[8].length())
		_pIOTACheckTestData->CONFIG_VERSION = atoi(listOfArgValue[8].c_str()) ? true : false;
}

void Ts_BravoIOTACheck::Execute()
{
	uint32_t rc = 0;
	Syn_Exception Exception(0);

	uint32_t iotaReplyLength(0);
	rc = _pSynModule->FpIotafind(IotaFindType::ReplyLength, iotaReplyLength, NULL, 0);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoIOTACheck::FpIotafind is failed(1)!");
		throw Exception;
		return;
	}
	if (0 == iotaReplyLength)
	{
		_pIOTACheckTestData->pass = false;
		return;
	}

	unsigned int length = iotaReplyLength + sizeof(vcsfw_reply_iota_find_t);
	uint8_t *arrIotaData = new uint8_t[length];
	rc = _pSynModule->FpIotafind(IotaFindType::GetData, iotaReplyLength, arrIotaData, length);
	if (0 != rc)
	{
		Exception.SetError(rc);
		Exception.SetDescription("BravoIOTACheck::FpIotafind is failed(2)!");
		throw Exception;
		return;
	}

	uint16_t nOffset = sizeof(vcsfw_reply_iota_find_t);
	do
	{
		string strIOTAType, strIOTAValue;
		vcsfw_reply_iota_find_hdr_t iota_find_hdr;
		memcpy(&iota_find_hdr, &arrIotaData[nOffset], sizeof(vcsfw_reply_iota_find_hdr_t));
		IotaParse(iota_find_hdr, arrIotaData, nOffset + sizeof(vcsfw_reply_iota_find_hdr_t), nOffset + iota_find_hdr.nbytes + sizeof(vcsfw_reply_iota_find_hdr_t), strIOTAType, strIOTAValue);
		if (0 != strIOTAValue.size() && 0 != strIOTAValue.size())
		{
			/*if (std::string::npos != strIOTAType.find("\n"))
			{
				std::size_t pos = strIOTAType.find("\n");
				string strIotaType = strIOTAType.substr(0, pos);
				string strFrameTag = strIOTAType.substr(pos + 1, strIOTAType.size() - pos - 1);
			}*/
			_pIOTACheckTestData->mapIOTAResults.insert(map<string, string>::value_type(strIOTAType, strIOTAValue));
		}

		nOffset = nOffset + iota_find_hdr.nbytes + sizeof(vcsfw_reply_iota_find_hdr_t);
	} while (nOffset < length);

	delete[] arrIotaData;
	arrIotaData = NULL;
}

void Ts_BravoIOTACheck::ProcessData()
{
	_pIOTACheckTestData->executed = true;

	unsigned int watchdog1 = 0, watchdog2 = 0;
	if (_pIOTACheckTestData->FrameBase_DIMS)
		watchdog1 += 1;
	if (_pIOTACheckTestData->FrameBase_FWBL)
		watchdog1 += 1;
	if (_pIOTACheckTestData->FrameBase_LNABL)
		watchdog1 += 1;
	if (_pIOTACheckTestData->FrameBase_REG16BLK)
		watchdog1 += 1;
	if (_pIOTACheckTestData->FrameBase_REG32BLK)
		watchdog1 += 1;
	if (_pIOTACheckTestData->FrameNav_LNABL)
		watchdog1 += 1;
	if (_pIOTACheckTestData->CONFIG_PSELECT)
		watchdog1 += 1;
	if (_pIOTACheckTestData->CONFIG_WOF_THRESHOLDS)
		watchdog1 += 1;
	if (_pIOTACheckTestData->CONFIG_VERSION)
		watchdog1 += 1;
	
	for (map<string, string>::iterator iter = _pIOTACheckTestData->mapIOTAResults.begin(); iter != _pIOTACheckTestData->mapIOTAResults.end(); iter++)
	{
		string strIOTAType = iter->first;
		string strIOTAValue = iter->second;
		string strFrameTag;
		if (std::string::npos != strIOTAType.find("\n"))
		{
			std::size_t pos = strIOTAType.find("\n");
			strFrameTag = strIOTAType.substr(pos + 1, strIOTAType.size() - pos - 1);
			strIOTAType = strIOTAType.substr(0, pos);
		}

		if ("FRAME_BASE" == strIOTAType)
		{
			if ("DIMS" == strFrameTag)
			{
				if (_pIOTACheckTestData->FrameBase_DIMS)
					watchdog2 += 1;
			}
			else if ("FW_BL" == strFrameTag)
			{
				if (_pIOTACheckTestData->FrameBase_FWBL)
					watchdog2 += 1;
			}
			else if ("LNA_BL" == strFrameTag)
			{
				if (_pIOTACheckTestData->FrameBase_LNABL)
					watchdog2 += 1;
			}
			else if ("REG16BLK" == strFrameTag)
			{
				if (_pIOTACheckTestData->FrameBase_REG16BLK)
					watchdog2 += 1;
			}
			else if ("REG32BLK" == strFrameTag)
			{
				if (_pIOTACheckTestData->FrameBase_REG32BLK)
					watchdog2 += 1;
			}
			else
			{
				continue;
			}
		}
		else if ("FRAME_NAV" == strIOTAType)
		{
			if ("LNA_BL" == strFrameTag)
			{
				if (_pIOTACheckTestData->FrameNav_LNABL)
					watchdog2 += 1;
			}
			else
			{
				continue;
			}
		}
		else if ("CONFIG_PSELECT" == strIOTAType)
		{
			if (_pIOTACheckTestData->CONFIG_PSELECT)
				watchdog2 += 1;
		}
		else if ("CONFIG_WOF_THRESHOLDS" == strIOTAType)
		{
			if (_pIOTACheckTestData->CONFIG_WOF_THRESHOLDS)
				watchdog2 += 1;
		}
		else if ("CONFIG_VERSION" == strIOTAType)
		{
			if (_pIOTACheckTestData->CONFIG_VERSION)
				watchdog2 += 1;
		}
		else
		{
			continue;
		}
	}

	if (watchdog1 == watchdog2 || 1 == (watchdog2 - watchdog1))
	{
		_pIOTACheckTestData->pass = true;
	}
	else
	{
		_pIOTACheckTestData->pass = false;
		_pSynDutUtils->_pDutTestResult->list_bincodes.push_back("152");
	}

	
	_pSynDutUtils->_pDutTestResult->map_teststep_ispass.insert(map<string, string>::value_type(_pIOTACheckTestData->data_name, _pIOTACheckTestData->pass ? "Pass" : "Fail"));
}

void Ts_BravoIOTACheck::CleanUp()
{
	CalculateTestTime(_pIOTACheckTestData->test_time);

	StoreTestData(_pIOTACheckTestData->data_name, static_cast<SynTestData*>(_pIOTACheckTestData));
}

void Ts_BravoIOTACheck::IotaParse(vcsfw_reply_iota_find_hdr_t iotafindheader, uint8_t *arrIota, uint32_t startPos, uint32_t endPos, string &oIOTAType, string &oIOTAValue)
{
	string strIOTAFrameTag, strFrameValue;
	switch (iotafindheader.itype)
	{
		case VCSFW_IOTA_ITYPE_FRAME_BASE:
			oIOTAType = "FRAME_BASE";
			IotaFrameParse(arrIota, startPos, endPos, strIOTAFrameTag, strFrameValue);
			oIOTAType += "\n" + strIOTAFrameTag;
			oIOTAValue = strFrameValue;
			break;
		case VCSFW_IOTA_ITYPE_FRAME_NAV:
			oIOTAType = "FRAME_NAV";
			IotaFrameParse(arrIota, startPos, endPos, strIOTAFrameTag, strFrameValue);
			oIOTAType += "\n" + strIOTAFrameTag;
			oIOTAValue = strFrameValue;
			break;
		case VCSFW_IOTA_ITYPE_FRAME_IMAGE:
			oIOTAType = "FRAME_IMAGE";
			IotaFrameParse(arrIota, startPos, endPos, strIOTAFrameTag, strFrameValue);
			oIOTAType += "\n" + strIOTAFrameTag;
			oIOTAValue = strFrameValue;
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_PSELECT:
			oIOTAType = "CONFIG_PSELECT";
			vcsfw_config_tag_pselect_t config_tag_pselect;
			memcpy(&config_tag_pselect, &(arrIota[startPos]), sizeof(vcsfw_config_tag_pselect_t));
			oIOTAValue += "diff_thresh:" + std::to_string(config_tag_pselect.diff_thresh) + "\n";
			oIOTAValue += "cnt_thresh:" + std::to_string(config_tag_pselect.cnt_thresh) + "\n";
			oIOTAValue += "delay_interval_msec:" + std::to_string(config_tag_pselect.delay_interval_msec) + "\n";
			oIOTAValue += "frame_avg:" + std::to_string(config_tag_pselect.frame_avg) + "\n";
			oIOTAValue += "pscan_col_size:" + std::to_string(config_tag_pselect.pscan_col_size) + "\n";
			oIOTAValue += "pscan_row_size:" + std::to_string(config_tag_pselect.pscan_row_size);
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_WOE:
			oIOTAType = "CONFIG_WOE";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_WOF_THRESHOLDS:
			oIOTAType = "CONFIG_WOF_THRESHOLDS";
			vcsfw_config_wof_thresholds_t config_wof_thresholds;
			memcpy(&config_wof_thresholds, &(arrIota[startPos]), sizeof(vcsfw_config_wof_thresholds_t));
			oIOTAValue += "signal:" + std::to_string(config_wof_thresholds.signal) + "\n";
			oIOTAValue += "hyst:" + std::to_string(config_wof_thresholds.hyst) + "\n";
			oIOTAValue += "reduction:" + std::to_string(config_wof_thresholds.reduction);
			break;
		case VCSFW_IOTA_ITYPE_CAL_REFERENCE:
			oIOTAType = "CAL_REFERENCE";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		case VCSFW_IOTA_ITYPE_BAD_PIXEL_MAP:
			oIOTAType = "BAD_PIXEL_MAP";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_VERSION:
			oIOTAType = "CONFIG_VERSION";
			vcsfw_config_version_t config_version;
			memcpy(&config_version, &(arrIota[startPos]), sizeof(vcsfw_config_version_t));
			oIOTAValue += "YYMMDD:" + std::to_string(config_version.config_id1) + "\n";
			oIOTAValue += "HHMMSS:" + std::to_string(config_version.config_id2) + "\n";
			oIOTAValue += "version:" + std::to_string(config_version.version);
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_NAV_SWIPE:
			oIOTAType = "CONFIG_NAV_SWIPE";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_NAV_TAP:
			oIOTAType = "CONFIG_NAV_TAP";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		case VCSFW_IOTA_ITYPE_CONFIG_FRAME_AVG:
			oIOTAType = "CONFIG_FRAME_AVG";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		case VCSFW_IOTA_ITYPE_FPPRESENT_PARAMS:
			oIOTAType = "FPPRESENT_PARAMS";
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
		default:
			oIOTAType = std::to_string(iotafindheader.itype);
			oIOTAValue = ConvertData(arrIota, startPos, endPos);
			break;
	}
}

void Ts_BravoIOTACheck::IotaFrameParse(uint8_t *arrIotaFrame, uint32_t startPos, uint32_t endPos, string &oFrameTag, string &oFrameData)
{
	vcsfw_frame_tag_t frametag;
	uint32_t reg16blkvalue(0);
	uint32_t reg32blkvalue(0);
	uint32_t WOFGainReg(0), WOFOffsetReg(0), WOFCtrlReg(0);

	memcpy(&frametag, &(arrIotaFrame[startPos]), sizeof(vcsfw_frame_tag_t));
	switch (frametag.tagid)
	{
		case VCSFW_FRAME_TAG_FW_BL:
			oFrameTag = "FW_BL";
			break;
		case VCSFW_FRAME_TAG_LNA_BL:
			oFrameTag = "LNA_BL";
			break;
		case VCSFW_FRAME_TAG_DM:
			oFrameTag = "DM";
			break;
		case VCSFW_FRAME_TAG_CDM:
			oFrameTag = "CDM";
			break;
		case VCSFW_FRAME_TAG_REG16BLK:
			oFrameTag = "REG16BLK";
			vcsfw_frame_tag_reg16blk_t frame_tag_reg16blk;
			memcpy(&frame_tag_reg16blk, &(arrIotaFrame[startPos + sizeof(vcsfw_frame_tag_t)]), sizeof(vcsfw_frame_tag_reg16blk_t));
			oFrameData += "regbase : " + std::to_string(frame_tag_reg16blk.regbase) + "\n";
			oFrameData += "nregs : " + std::to_string(frame_tag_reg16blk.nregs) + "\n";
			memcpy(&reg16blkvalue, &(arrIotaFrame[startPos + sizeof(vcsfw_frame_tag_t) + sizeof(vcsfw_frame_tag_reg16blk_t)]), sizeof(uint32_t));
			oFrameData += "WOF_AMP_4_GAIN : " + std::to_string(GetBitsValue(reg16blkvalue, 0, 3)) + "\n";
			oFrameData += "WOF_AMP_3_GAIN : " + std::to_string(GetBitsValue(reg16blkvalue, 3, 3)) + "\n";
			oFrameData += "WOF_AMP_2_GAIN : " + std::to_string(GetBitsValue(reg16blkvalue, 6, 1)) + "\n";
			oFrameData += "RESERVED : " + std::to_string(GetBitsValue(reg16blkvalue, 7, 1)) + "\n";
			oFrameData += "WOF_TX_CAP : " + std::to_string(GetBitsValue(reg16blkvalue, 8, 3)) + "\n";
			oFrameData += "WOF_CAL_POLARITY : " + std::to_string(GetBitsValue(reg16blkvalue, 11, 1)) + "\n";
			oFrameData += "RESERVED : " + std::to_string(GetBitsValue(reg16blkvalue, 12, 20));
			break;
		case VCSFW_FRAME_TAG_REG32BLK:
			oFrameTag = "REG32BLK";
			_pSynModule->FpGetWOFRegister(WOFGainReg,WOFOffsetReg,WOFCtrlReg);
			vcsfw_frame_tag_reg32blk_t frame_tag_reg32blk;
			memcpy(&frame_tag_reg32blk, &(arrIotaFrame[startPos + sizeof(vcsfw_frame_tag_t)]), sizeof(vcsfw_frame_tag_reg32blk_t));
			oFrameData += "regbase : " + std::to_string(frame_tag_reg32blk.regbase) + "\n";
			oFrameData += "nregs : " + std::to_string(frame_tag_reg32blk.nregs) + "\n";
			memcpy(&reg32blkvalue, &(arrIotaFrame[startPos + sizeof(vcsfw_frame_tag_t) + sizeof(vcsfw_frame_tag_reg32blk_t)]), sizeof(uint32_t));
			if (WOFOffsetReg == frame_tag_reg32blk.regbase)
			{
				oFrameData += "RXP : " + std::to_string(GetBitsValue(reg32blkvalue, 0, 11)) + "\n";
				oFrameData += "RESERVED : " + std::to_string(GetBitsValue(reg32blkvalue,11, 1)) + "\n";
				oFrameData += "RXM : " + std::to_string(GetBitsValue(reg32blkvalue, 12, 11)) + "\n";
				oFrameData += "RESERVED : " + std::to_string(GetBitsValue(reg32blkvalue, 23, 9));
			}
			else if (WOFCtrlReg == frame_tag_reg32blk.regbase)
			{
				oFrameData += "FD_ACCUM : " + std::to_string(GetBitsValue(reg32blkvalue, 0, 1)) + "\n";
				oFrameData += "FU_ACCUM : " + std::to_string(GetBitsValue(reg32blkvalue, 1, 1)) + "\n";
				oFrameData += "FD_SAMPLES : " + std::to_string(GetBitsValue(reg32blkvalue, 2, 10)) + "\n";
				oFrameData += "FU_SAMPLES : " + std::to_string(GetBitsValue(reg32blkvalue, 12, 10)) + "\n";
				oFrameData += "FH_SAMPLES : " + std::to_string(GetBitsValue(reg32blkvalue, 22, 10));
			}
			break;
		case VCSFW_FRAME_TAG_DIMS:
			oFrameTag = "DIMS";
			break;
		case VCSFW_FRAME_TAG_ACQOPT:
			oFrameTag = "ACQOPT";
			break;
		case VCSFW_FRAME_TAG_XSREG8BLK:
			oFrameTag = "XSREG8BLK";
			break;
		case VCSFW_FRAME_TAG_STRIDEBLK:
			oFrameTag = "STRIDEBLK";
			break;
		case VCSFW_FRAME_TAG_STATS:
			oFrameTag = "STATS";
			break;
		case VCSFW_FRAME_TAG_ACQCFG:
			oFrameTag = "ACQCFG";
			break;
		default:
			oFrameTag = std::to_string(frametag.tagid);
			break;
	}

	oFrameData = ConvertData(arrIotaFrame, startPos + sizeof(vcsfw_frame_tag_t), endPos);
}


string Ts_BravoIOTACheck::ConvertData(uint8_t *arrData, uint32_t startPos, uint32_t endPos)
{
	string strContenet;
	unsigned int length = endPos - startPos;
	unsigned int templength = length % 16;
	if (length <= 16)
	{
		string strLineValue = "";
		for (unsigned int i = startPos; i < endPos; i++)
		{
			string strValue;
			char buffer[5];
			sprintf(buffer, "%02X", arrData[i]);
			strValue = buffer;
			strLineValue += strValue + " ";
		}

		strContenet += strLineValue + "\n";
	}
	else
	{
		int k = 0;
		string strLineValue = "";
		for (unsigned int i = startPos; i < endPos; i++)
		{
			string strValue;
			char buffer[5];
			sprintf(buffer, "%02X", arrData[i]);
			strValue = buffer;
			strLineValue += strValue + " ";
			k++;
			if (16 == k)
			{
				strContenet += strLineValue + "\n";
				strLineValue = "";
				length -= 16;
				k = 0;
			}
			else if (length < 16)
			{
				strLineValue = "";
				for (unsigned int i = endPos - length; i < endPos; i++)
				{
					string strValue;
					char buffer[5];
					sprintf(buffer, "%02X", arrData[i]);
					strValue = buffer;
					strLineValue += strValue + " ";
				}

				strContenet += strLineValue + "\n";
				break;
			}
		}
	}

	return strContenet;
}

uint32_t Ts_BravoIOTACheck::GetBitsValue(uint32_t value, uint8_t startIndex, uint32_t length)
{
	std::bitset<32> valueBits(value);

	std::bitset<32> resultBits(0);
	unsigned int currentIndex(0);
	for (unsigned i = startIndex; i < startIndex + length; ++i)
	{
		resultBits[currentIndex] = valueBits[i];
		currentIndex++;
	}

	uint32_t result = (uint32_t)resultBits.to_ulong();

	return result;
}