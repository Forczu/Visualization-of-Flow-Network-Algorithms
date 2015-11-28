#include "AddWeightToEdgeDialog.h"
#include "Strings.h"
#include "QMessageBox"

AddWeightToEdgeDialog::AddWeightToEdgeDialog(int from, int to)
{
	ui.setupUi(this);
	ui.sourceEdit->setText(Strings::Instance().get(VERTEX_WITH_ID).arg(from));
	ui.targetEdit->setText(Strings::Instance().get(VERTEX_WITH_ID).arg(to));
	connect(ui.okButton, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
	connect(ui.cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
}

AddWeightToEdgeDialog::~AddWeightToEdgeDialog()
{
}

void AddWeightToEdgeDialog::okClicked()
{
	if (getFlow() <= getCapacity())
	{
		accept();
		return;
	}
	QMessageBox msgBox;
	msgBox.setWindowTitle(Strings::Instance().get(WRONG_VALUE));
	msgBox.setText(Strings::Instance().get(FLOW_GREATER_THAN_CAPACITY_MSGBX));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void AddWeightToEdgeDialog::cancelClicked()
{
	reject();
}
