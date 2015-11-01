#ifndef CREATENEWGRAPHDIALOG_H
#define CREATENEWGRAPHDIALOG_H

#include <map>
#include <QDialog>
#include <QTextCodec>
#include "ui_CreateNewGraphDialog.h"
#include "Orders.h"
#include "Weights.h"

class CreateNewGraphDialog : public QDialog
{
	Q_OBJECT

	std::map<Order, QString>	_orderMap;
	std::map<Weight, QString>	_weightMap;
	bool _confirmed;
public:
	CreateNewGraphDialog(int newTabIndex, QWidget *parent = 0);
	~CreateNewGraphDialog();

	inline QString getName() const
	{
		return ui.graphNameText->text();
	}
	Order getOrder() const;
	Weight getWeighted() const;

	inline bool Confirmed() const
	{
		return _confirmed;
	}

private:
	Ui::CreateNewGraphDialog ui;

private slots:
	void okButtunPushed();
	void cancelButtonPushed();
	void updateOrder(const QString &);
};

#endif // CREATENEWGRAPHDIALOG_H
