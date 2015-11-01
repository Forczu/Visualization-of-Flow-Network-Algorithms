#include "CreateNewGraphDialog.h"

CreateNewGraphDialog::CreateNewGraphDialog(int newTabIndex, QWidget *parent)
: QDialog(parent), _confirmed(false)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	ui.setupUi(this);
	ui.graphNameText->setText(QString("Graf %1").arg(newTabIndex));
	
	_orderMap[Order::Directed] = "Graf skierowany";
	_orderMap[Order::Undirected] = "Graf nieskierowany";
	_orderMap[Order::FlowNetwork] = "Sieæ przep³ywowa";
	_weightMap[Weight::Weighted] = QString::fromUtf8("Graf wa¿ony");
	_weightMap[Weight::Unwieghted] = QString::fromUtf8("Graf niewa¿ony");

	std::for_each(_orderMap.begin(), _orderMap.end(), [&](std::pair<Order, QString> const & value)
	{
		ui.orderComboBox->addItem(value.second);
	});
	std::for_each(_weightMap.begin(), _weightMap.end(), [&](std::pair<Weight, QString> const & value)
	{
		ui.weightComboBox->addItem(value.second);
	});

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okButtunPushed()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPushed()));
	connect(ui.orderComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(updateOrder(const QString &)));
}

CreateNewGraphDialog::~CreateNewGraphDialog()
{
}

Order CreateNewGraphDialog::getOrder() const
{
	for (auto item : _orderMap)
	{
		if (item.second == ui.orderComboBox->currentText())
		{
			return item.first;
		}
	}
}

Weight CreateNewGraphDialog::getWeighted() const
{
	for (auto item : _weightMap)
	{
		if (item.second == ui.weightComboBox->currentText())
		{
			return item.first;
		}
	}
}

void CreateNewGraphDialog::okButtunPushed()
{
	_confirmed = true;
	close();
}

void CreateNewGraphDialog::cancelButtonPushed()
{
	_confirmed = false;
	close();
}

void CreateNewGraphDialog::updateOrder(const QString & str)
{
	if (str == "Sieæ przep³ywowa")
	{
		ui.weightComboBox->setEnabled(false);
	}
	else
	{
		ui.weightComboBox->setEnabled(true);
	}
}
