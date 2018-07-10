#include "Designer.h"

Studio::Softer::Windows::Designer::Designer(QWidget *parent) 
	: QMainWindow(parent), ui(new Ui::Designer)
{
	ui->setupUi(this);
	ui->toolBar->setMinimumHeight(40);
	ui->toolBarParams->setMinimumHeight(35);
	ui->toolBox->setMinimumWidth(40);
	ui->statusBar->setMinimumHeight(25);
}

Studio::Softer::Windows::Designer::~Designer()
{
	delete ui;
}
