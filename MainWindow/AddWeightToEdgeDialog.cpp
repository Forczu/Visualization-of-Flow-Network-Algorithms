#include "AddWeightToEdgeDialog.h"

AddWeightToEdgeDialog::AddWeightToEdgeDialog(int from, int to) : _confirmed(false)
{
	ui.setupUi(this);
	ui.sourceEdit->setText(QString("Wierzcho³ek %1").arg(from));
	ui.targetEdit->setText(QString("Wierzcho³ek %1").arg(to));
	connect(ui.okButton, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
	connect(ui.cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
}

AddWeightToEdgeDialog::~AddWeightToEdgeDialog()
{
}

void AddWeightToEdgeDialog::okClicked()
{
	_confirmed = true;
	close();
}

void AddWeightToEdgeDialog::cancelClicked()
{
	_confirmed = false;
	close();
}
