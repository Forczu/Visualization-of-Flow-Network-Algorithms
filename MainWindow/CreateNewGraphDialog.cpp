#include "CreateNewGraphDialog.h"
#include "FlowNetwork.h"
#include "WeightedEdgeStrategy.h"
#include "UnweightedEdgeStrategy.h"
#include "UndirectedGraphImage.h"

CreateNewGraphDialog::CreateNewGraphDialog(int newTabIndex, QWidget *parent)
: QDialog(parent), _result(DialogCode::Rejected)
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	ui.setupUi(this);
	ui.graphNameText->setText(QString("Graf %1").arg(newTabIndex));

	_graphTypeMap["Sieæ przep³ywowa"] = FlowNetwork::getInstance;
	_graphTypeMap["Graf skierowany"] = DirectedGraphImage::getInstance;
	_graphTypeMap["Graf nieskierowany"] = UndirectedGraphImage::getInstance;
	_weightStrategyMap["Graf wa¿ony"] = WeightedEdgeStrategy::getInstance;
	_weightStrategyMap["Graf niewa¿ony"] = UnweightedEdgeStrategy::getInstance;

	for (QString key : _graphTypeMap.keys())
	{
		ui.orderComboBox->addItem(key);
	}
	for (QString key : _weightStrategyMap.keys())
	{
		ui.weightComboBox->addItem(key);
	}
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okButtunPushed()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPushed()));
	connect(ui.orderComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(updateOrder(const QString &)));
	updateOrder(ui.orderComboBox->currentText());
}

CreateNewGraphDialog::~CreateNewGraphDialog()
{
}

void CreateNewGraphDialog::okButtunPushed()
{
	_result = QDialog::Accepted;
	close();
}

void CreateNewGraphDialog::cancelButtonPushed()
{
	_result = QDialog::Rejected;
	close();
}

void CreateNewGraphDialog::updateOrder(const QString & str)
{
	if (str == "Sieæ przep³ywowa")
	{
		ui.weightComboBox->setCurrentIndex(1);
		ui.weightComboBox->setEnabled(false);
	}
	else
	{
		ui.weightComboBox->setEnabled(true);
	}
}
