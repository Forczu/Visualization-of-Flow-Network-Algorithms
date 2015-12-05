#ifndef ADDWEIGHTTOEDGEDIALOG_H
#define ADDWEIGHTTOEDGEDIALOG_H

#include <QDialog>
#include "ui_AddWeightToEdgeDialog.h"

class AddWeightToEdgeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddWeightToEdgeDialog(int from, int to);
	~AddWeightToEdgeDialog();

	inline int getFlow() const
	{
		return ui.flowBox->value();
	}
	inline int getCapacity() const
	{
		return ui.capacityBox->value();
	}

private:
	Ui::AddWeightToEdgeDialog ui;

private slots:
	void okClicked();
	void cancelClicked();
};

#endif // ADDWEIGHTTOEDGEDIALOG_H
