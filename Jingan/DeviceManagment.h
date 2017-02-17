#ifndef DEVICEMANAGMENT_H
#define DEVICEMANAGMENT_H

#include <QWidget>
#include "ui_DeviceManagment.h"

#include "LocalSettings.h"

class DeviceManagment : public QDialog
{
	Q_OBJECT

public:
	DeviceManagment(QWidget *parent = 0);
	~DeviceManagment();

	void showEvent(QShowEvent * event);

public Q_SLOTS:

	void ButtonBoxClicked(QAbstractButton * button);

private:

	Ui::DeviceManagment ui;
};

#endif // DEVICEMANAGMENT_H
