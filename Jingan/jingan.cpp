#include "jingan.h"

Jingan::Jingan(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.actionExit, SIGNAL(triggered(bool)), this, SLOT(Exit()));
}

Jingan::~Jingan()
{

}

void Jingan::Exit()
{
	QCoreApplication::exit();
}
