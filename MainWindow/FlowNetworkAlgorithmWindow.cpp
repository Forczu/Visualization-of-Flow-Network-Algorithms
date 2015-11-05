#include "FlowNetworkAlgorithmWindow.h"
#include <math.h>
#include "IAlgorithm.h"

FlowNetworkAlgorithmWindow::FlowNetworkAlgorithmWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	createConnections();
	ui.mainNetworkView->setInteractive(false);
	ui.residualNetworkView->setInteractive(false);
}

FlowNetworkAlgorithmWindow::~FlowNetworkAlgorithmWindow()
{

}

void FlowNetworkAlgorithmWindow::setMainNetwork(FlowNetwork * network)
{
	_mainNetwork = network;
	QRectF sceneRect = network->childrenBoundingRect();
	QRectF viewRect = ui.mainNetworkView->geometry();
	float widthScale = viewRect.width() / sceneRect.width();
	float heightScale = viewRect.height() / sceneRect.height();
	float scaleFactor = std::min(widthScale, heightScale);
	if (scaleFactor < 1.0f)
	{
		scaleFactor -= 0.05f;	// odrobina przestrzeni przy ramce
		ui.mainNetworkView->scale(scaleFactor, scaleFactor);
		network->updateScale(scaleFactor);
	}
	ui.mainNetworkView->setScene(new QGraphicsScene(sceneRect));
	ui.residualNetworkView->setScene(new QGraphicsScene(sceneRect));
	ui.mainNetworkView->scene()->addItem(network);
}

void FlowNetworkAlgorithmWindow::setAlgorithm(FlowNetworkAlgorithm * algorithm)
{
	_algorithm = algorithm;
}

void FlowNetworkAlgorithmWindow::createConnections()
{
	connect(ui.nextStepButton, SIGNAL(clicked()), this, SLOT(makeNextStep()));
	connect(ui.finishAlgorithmButton, SIGNAL(clicked()), this, SLOT(finish()));
}

void FlowNetworkAlgorithmWindow::makeNextStep()
{
	static bool left = false;
	if (left)
	{
		QList<int> path = _algorithm->findAugumentingPath();
		_algorithm->increaseFlow(path);
	}
	else
	{
		ui.residualNetworkView->scene()->clear();
		FlowNetwork * residualNewtork = _algorithm->makeResidualNetwork();
		ui.residualNetworkView->scene()->addItem(residualNewtork);
	}
}

void FlowNetworkAlgorithmWindow::finish()
{
}
