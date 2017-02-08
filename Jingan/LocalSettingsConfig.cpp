#include "LocalSettingsConfig.h"

#include <QTextStream>

#include <iostream>

LocalSettingsConfig::LocalSettingsConfig(QString strConfigFilePath)
:_strFilePath(strConfigFilePath)
, q_DomDocument("SynLocalSettingXML")
, q_DomRootNode()
{
}

LocalSettingsConfig::~LocalSettingsConfig()
{
}

uint32_t LocalSettingsConfig::CreateLSConfigInstance(OperationType Type, LocalSettingsConfig * &opLSConfigInstance, QString strConfigFilePath)
{
	opLSConfigInstance = NULL;

	if (QString("") == strConfigFilePath)
	{
		cout << "Error:LocalSettingsConfig::CreateLSConfigInstance() - strXMLFilePath is NULL!" << endl;
		return 1;
	}

	opLSConfigInstance = new LocalSettingsConfig(strConfigFilePath);
	uint32_t rc = opLSConfigInstance->Initialize(Type);
	if (0 != rc)
	{
		delete opLSConfigInstance;
		opLSConfigInstance = NULL;
	}

	return rc;
}

uint32_t LocalSettingsConfig::Initialize(OperationType Type)
{
	QFile xmlFile(_strFilePath);

	if (Read == Type)
	{
		//q_File.
		if (xmlFile.exists())
		{
			if (!xmlFile.open(QFile::ReadOnly | QFile::Text))
			{
				cout << "LocalSettingsConfig::Initialize() - xmlFile.open is failed!" << endl;
				return 1;
			}

			QString error;
			int row = 0, column = 0;
			if (!q_DomDocument.setContent(&xmlFile, false, &error, &row, &column))
			{
				cout << "LocalSettingsConfig::Initialize() - q_DomDocument.setContent is failed!" << endl;
				return 2;
			}

			if (q_DomDocument.isNull())
			{
				cout << "LocalSettingsConfig::Initialize() - q_DomDocument is null!" << endl;
				return 3;
			}

			QDomElement root = q_DomDocument.documentElement();
			q_DomRootNode = root;

			xmlFile.close();
		}
		else
		{
			QDomProcessingInstruction instruction = q_DomDocument.createProcessingInstruction(QString("xml"), QString("version = \"1.0\" encoding = \"UTF-8\""));
			q_DomDocument.appendChild(instruction);
		}
	}
	else
	{
		QDomProcessingInstruction instruction = q_DomDocument.createProcessingInstruction(QString("xml"), QString("version = \"1.0\" encoding = \"UTF-8\""));
		q_DomDocument.appendChild(instruction);
	}

	return 0;
}

uint32_t LocalSettingsConfig::GetLocalSettings(LocalSettingConfig_t &oLocalSettings)
{
	uint32_t rc(0);

	QDomElement rootNode = q_DomDocument.documentElement();
	if (rootNode.isNull())
	{
		cout << "Error:LocalSettingsConfig::GetLocalSettings() - rootNode is null!" << endl;
		return 1;
	}

	QDomNodeList ListOfChildNode = rootNode.childNodes();
	if (ListOfChildNode.isEmpty())
	{
		cout << "Error:Syn_LocalSettingConfig::GetLocalSettings() - ListOfChildNode is null!" << endl;
		return 2;
	}

	oLocalSettings.list_serialnumber.clear();
	int iLength = ListOfChildNode.length();
	for (int i = 1; i <= iLength; i++)
	{
		QDomNode dom_node = ListOfChildNode.item(i - 1);
		QDomElement element = dom_node.toElement();

		QString strTagName(element.tagName());
		if (QString("SysConfigFile") == strTagName)
		{
			oLocalSettings.strConfigFilePath = element.text();
		}
		if (QString("LogFilePath") == strTagName)
		{
			oLocalSettings.strLogFilePath = element.text();
		}
		else if (QString("DeviceType") == strTagName)
		{
			oLocalSettings.strDeviceType = element.text().toStdString();
		}
		else if (QString("TestEngine") == strTagName.left(10))
		{
			string strDeviceSerialNumber = element.text().toStdString();
			oLocalSettings.list_serialnumber.push_back(strDeviceSerialNumber);
		}
		else
		{
			continue;
		}
	}

	return true;
}

uint32_t LocalSettingsConfig::SetLocalSettings(LocalSettingConfig_t iLocalSettings)
{
	uint32_t rc(0);

	//delete all
	QDomElement root = q_DomDocument.documentElement();
	if (!root.isNull())
	{
		q_DomDocument.removeChild(root);
	}

	//root
	QDomElement rootNode = q_DomDocument.createElement("LocalSettings");
	q_DomDocument.appendChild(rootNode);

	//ConfigFile
	QDomElement sysConfigFile_Node = q_DomDocument.createElement("SysConfigFile");
	QString strSysConfigFilePath = iLocalSettings.strConfigFilePath;
	QDomText sysConfigFile_TextNode = q_DomDocument.createTextNode(strSysConfigFilePath);
	rootNode.appendChild(sysConfigFile_Node);
	sysConfigFile_Node.appendChild(sysConfigFile_TextNode);

	//LogFilePath
	QDomElement logFilePath_Node = q_DomDocument.createElement("LogFilePath");
	QString strLogFilePath = iLocalSettings.strLogFilePath;
	QDomText logFile_TextNode = q_DomDocument.createTextNode(strLogFilePath);
	rootNode.appendChild(logFilePath_Node);
	logFilePath_Node.appendChild(logFile_TextNode);

	//Engine counts
	unsigned int engine_counts = iLocalSettings.list_serialnumber.size();
	QDomElement engineCounts_Node = q_DomDocument.createElement("EngineNumbers");
	QString strEngineCounts(QString::number(engine_counts));
	QDomText engineCounts_TextNode = q_DomDocument.createTextNode(strEngineCounts);
	rootNode.appendChild(engineCounts_Node);
	engineCounts_Node.appendChild(engineCounts_TextNode);
	for (unsigned int i = 1; i <= engine_counts; i++)
	{
		QString strEngineTitle(QString("TestEngine") + QString::number(i));
		QDomElement testEngine_Node = q_DomDocument.createElement(strEngineTitle);
		QString strSerialNumber(QString::fromStdString(iLocalSettings.list_serialnumber[i - 1]));
		QDomText testEngine_TextNode = q_DomDocument.createTextNode(strSerialNumber);
		rootNode.appendChild(testEngine_Node);
		testEngine_Node.appendChild(testEngine_TextNode);
	}

	//DeviceType
	QDomElement deviceType_Node = q_DomDocument.createElement("DeviceType");
	QString strDeviceType(QString::fromStdString(iLocalSettings.strDeviceType));
	QDomText deviceType_TextNode = q_DomDocument.createTextNode(strDeviceType);
	rootNode.appendChild(deviceType_Node);
	deviceType_Node.appendChild(deviceType_TextNode);

	//Save
	QFile qFile(_strFilePath);
	if (!qFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		return false;

	QTextStream qTextStream(&qFile);

	qTextStream.setCodec("UTF-8");

	q_DomDocument.save(qTextStream, 4, QDomNode::EncodingFromTextStream);

	qFile.close();

	return true;
}