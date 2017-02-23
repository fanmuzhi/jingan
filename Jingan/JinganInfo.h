#ifndef JINGANINFO_H
#define JINGANINFO_H

#include <QWidget>
#include "ui_JinganInfo.h"

class JinganInfo : public QWidget
{
	Q_OBJECT

public:
	JinganInfo(QWidget *parent = 0);
	~JinganInfo();

	void SetInfo();

private:
	Ui::JinganInfo ui;
};

#endif // JINGANINFO_H
