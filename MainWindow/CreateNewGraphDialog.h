#ifndef CREATENEWGRAPHDIALOG_H
#define CREATENEWGRAPHDIALOG_H

#include <map>
#include <QDialog>
#include "ui_CreateNewGraphDialog.h"

class CreateNewGraphDialog : public QDialog
{
	Q_OBJECT
	Ui::CreateNewGraphDialog ui;
public:
	explicit CreateNewGraphDialog(int newTabIndex, QWidget *parent = 0);
	QString getGraphName() const { return ui.graphNameText->text(); }

private slots:
	void okButtunPushed();
	void cancelButtonPushed();
};

#endif // CREATENEWGRAPHDIALOG_H
