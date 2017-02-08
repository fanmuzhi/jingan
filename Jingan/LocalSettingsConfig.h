#ifndef LOCALSETTINGSCONFIG_H
#define LOCALSETTINGSCONFIG_H

//Qt
#include "QString.h"
#include <QtXml/QDomDocument>
#include <qfile.h>
#include <qdom.h>

#include <vector>
#include <string>
#include "stdint.h"

using namespace std;

struct LocalSettingConfig_t
{
	QString strConfigFilePath;

	QString strLogFilePath;

	string strDeviceType;//M5(default), MPC04

	vector<string> list_serialnumber;
};

class LocalSettingsConfig
{
public:

	enum OperationType{ Read, Save };

	static uint32_t CreateLSConfigInstance(OperationType Type, LocalSettingsConfig * &opLSConfigInstance, QString strConfigFilePath = QString("LocalSettings.xml"));

	//LocalSettingsConfig();
	~LocalSettingsConfig();

	uint32_t GetLocalSettings(LocalSettingConfig_t &oLocalSettings);

	uint32_t SetLocalSettings(LocalSettingConfig_t iLocalSettings);

private:

	LocalSettingsConfig(QString strConfigFilePath);

	uint32_t Initialize(OperationType Type);

private:
	
	QString _strFilePath;

	QDomDocument q_DomDocument;

	QDomElement q_DomRootNode;
};

#endif // LOCALSETTINGSCONFIG_H
