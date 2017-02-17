#ifndef LOCALSETTINGS_H
#define LOCALSETTINGS_H

#include <QWidget>
#include <QDialog>
#include "ui_LocalSettings.h"

#include "syn_devicemanager.h"

#include "LocalSettingsConfig.h"

class LocalSettings : public QDialog
{
	Q_OBJECT

public:
	LocalSettings(QWidget *parent = 0);
	~LocalSettings();

	int GetEngineRowIndex();

	void GetSerialNumberList(vector<string> &ListOfSerialNumber);

	void SelectDevice(const QString strSerialNumber);

public Q_SLOTS:

	void SelectConfigFile();

	void SelectLogFilePath();

	void ModifyTestEngineCounts(int i);

	void CreateDeviceManagment();

	void ButtonBoxClicked(QAbstractButton * button);

	void Confirm();

protected:

	void Init();

private:
	Ui::LocalSettings ui;

	syn_devicemanager *_pDeviceManager;

	LocalSettingConfig_t _LocalSettingsConfig;
};

#endif // LOCALSETTINGS_H
