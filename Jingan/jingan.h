#ifndef JINGAN_H
#define JINGAN_H

#include <QtWidgets/QMainWindow>
#include "ui_jingan.h"

class Jingan : public QMainWindow
{
	Q_OBJECT

public:
	Jingan(QWidget *parent = 0);
	~Jingan();

private:
	Ui::JinganClass ui;
};

#endif // JINGAN_H
