#include "AddWeightToEdgeDialog.h"
#include "Strings.h"

AddWeightToEdgeDialog::AddWeightToEdgeDialog(int from, int to) : _confirmed(false)
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
	_confirmed = true;
	close();
}

void AddWeightToEdgeDialog::cancelClicked()
{
	_confirmed = false;
	close();
}
