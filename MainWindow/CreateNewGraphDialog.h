#ifndef CREATENEWGRAPHDIALOG_H
#define CREATENEWGRAPHDIALOG_H

#include <map>
#include <QDialog>
#include <QTextCodec>
#include "ui_CreateNewGraphDialog.h"
#include <QMap>
#include "GraphImage.h"

class CreateNewGraphDialog : public QDialog
{
	Q_OBJECT
	Ui::CreateNewGraphDialog ui;
	QMap<QString, GraphCreateFunc> _graphTypeMap;
	QMap<QString, bool> _weightMap;
	DialogCode _result;
public:
	CreateNewGraphDialog(int newTabIndex, QWidget *parent = 0);
	~CreateNewGraphDialog();

	inline QString getName() const
	{
		return ui.graphNameText->text();
	}
	inline DialogCode getResult() const
	{
		return _result;
	}
	inline bool isWeighted()
	{
		return _weightMap[ui.weightComboBox->currentText()];
	}
	inline GraphCreateFunc getGraphFunc()
	{
		return _graphTypeMap[ui.orderComboBox->currentText()];
	}

private slots:
	void okButtunPushed();
	void cancelButtonPushed();
	void updateOrder(const QString &);
};

#endif // CREATENEWGRAPHDIALOG_H
