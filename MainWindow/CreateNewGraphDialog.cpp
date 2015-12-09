#include "CreateNewGraphDialog.h"

CreateNewGraphDialog::CreateNewGraphDialog(int newTabIndex, QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);
	ui.graphNameText->setText(QString("Graf %1").arg(newTabIndex));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okButtunPushed()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPushed()));
}

void CreateNewGraphDialog::okButtunPushed()
{
	accept();
}

void CreateNewGraphDialog::cancelButtonPushed()
{
	reject();
}
