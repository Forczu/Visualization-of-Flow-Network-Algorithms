#include "FlowNetworkAlgorithmWindow.h"
#include <math.h>
#include "IAlgorithm.h"
#include "GraphScene.h"

FlowNetworkAlgorithmWindow::FlowNetworkAlgorithmWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	createConnections();
	QGraphicsScene * scene = GraphScene::getInstance();
	ui.mainNetworkView->addScene(scene);
	ui.residualNetworkView->addScene(scene);
	ui.mainNetworkView->setInteractive(false);
	ui.residualNetworkView->setInteractive(false);
	ui.mainNetworkView->setMouseTracking(false);
	ui.residualNetworkView->setMouseTracking(false);
	ui.mainNetworkView->setBlocked(true);
	ui.residualNetworkView->setBlocked(true);
}

FlowNetworkAlgorithmWindow::~FlowNetworkAlgorithmWindow()
{
}

void FlowNetworkAlgorithmWindow::scaleViews(FlowNetwork * network)
{
	QRectF networkRect = network->childrenBoundingRect();
	QRectF viewRect = ui.mainNetworkView->geometry();
	float widthScale = viewRect.width() / networkRect.width();
	float heightScale = viewRect.height() / networkRect.height();
	_scaleFactor = std::min(widthScale, heightScale);
	if (_scaleFactor < 1.0f)
	{
		_scaleFactor -= 0.05f;	// odrobina przestrzeni przy ramce
		ui.mainNetworkView->scale(_scaleFactor, _scaleFactor);
		ui.residualNetworkView->scale(_scaleFactor, _scaleFactor);
	}
	network->updateScale(_scaleFactor);
	_dx = std::max(
			static_cast<float>(networkRect.width() * 1.20),
			static_cast<float>(network->sceneBoundingRect().width()));
	ui.mainNetworkView->setGraphImage(network);
	ui.mainNetworkView->centerOn(network);
	network->unselectAll();
	_network = network;
	ui.residualNetworkView->centerOn(_network->pos().x() + _dx, _network->pos().y());
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
		FlowNetwork * residualNewtork = _algorithm->makeResidualNetwork(_network);
		residualNewtork->updateScale(_scaleFactor);
		residualNewtork->unselectAll();
		ui.residualNetworkView->scene()->addItem(residualNewtork);
		residualNewtork->setPos(_dx, residualNewtork->pos().y());
		ui.residualNetworkView->centerOn(residualNewtork);
		ui.residualNetworkView->viewport()->update();
	}
	left = !left;
}

void FlowNetworkAlgorithmWindow::finish()
{
}