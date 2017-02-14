#pragma once

//std
#include <exception>
#include <cstdint>
#include <string>
using namespace std;



#define ERROR_TESTENGINE_STATE		0x6001
#define ERROR_TESTENGINE_DATA		0x6002
#define ERROR_TESTSTEP_UNDEFINE		0x6003

#define ERROR_DUTUTILS_NULL			0x6004
#define ERROR_BRAVOMODULE_NULL		0x6005

#define ERROR_ENGINELOG_FOLDER		0x6101
#define ERROR_ENGINELOG_FILE		0x6102




class Syn_Exception : public exception
{
public:

	Syn_Exception(uint32_t iErrorNumber) 
		:_ErrorNumber(iErrorNumber), _strDescription("")
	{
	}
	Syn_Exception(uint32_t iErrorNumber, string strDescription)
		:_ErrorNumber(iErrorNumber), _strDescription(strDescription)
	{
	}

	inline void SetError(const uint32_t error)
	{
		_ErrorNumber = error;
	}

	inline uint32_t GetError()
	{
		return _ErrorNumber;
	}

	inline void	SetDescription(const string &strDescription)
	{
		_strDescription = strDescription;
	}

	inline string GetDescription() const
	{
		return _strDescription + "(" + to_string(_ErrorNumber) + ")";
	}

	inline const char* what()
	{
		std::string descrip = GetDescription();
		return descrip.c_str();
	}

private:

	string	 _strDescription;
	uint32_t _ErrorNumber;
};