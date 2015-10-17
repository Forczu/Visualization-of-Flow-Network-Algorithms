#ifndef ADDWEIGHTTOEDGEDIALOG_H
#define ADDWEIGHTTOEDGEDIALOG_H

#include <QDialog>
#include "ui_AddWeightToEdgeDialog.h"

class AddWeightToEdgeDialog : public QDialog
{
	Q_OBJECT

public:
	AddWeightToEdgeDialog(int from, int to);
	~AddWeightToEdgeDialog();

	inline int getWeight() const
	{
		return ui.weightBox->value();
	}
	inline bool isConfirmed() const
	{
		return _confirmed;
	}

private:
	Ui::AddWeightToEdgeDialog ui;
	bool _confirmed;

private slots:
	void okClicked();
	void cancelClicked();
};

#endif // ADDWEIGHTTOEDGEDIALOG_H
