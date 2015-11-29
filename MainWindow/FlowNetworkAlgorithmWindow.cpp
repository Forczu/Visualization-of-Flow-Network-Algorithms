#include "FlowNetworkAlgorithmWindow.h"
#include <math.h>
#include "IAlgorithm.h"
#include "GraphScene.h"
#include "Strings.h"

FlowNetworkAlgorithmWindow::FlowNetworkAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent)
: QDialog(parent), _algorithm(algorithm), _network(network), _step(0)
{
	ui.setupUi(this);
	createConnections();
	QGraphicsScene * scene = GraphScene::getInstance();
	configureView(ui.mainNetworkView, scene);
	configureView(ui.residualNetworkView, scene);
}

FlowNetworkAlgorithmWindow::~FlowNetworkAlgorithmWindow()
{
}

void FlowNetworkAlgorithmWindow::configureView(GraphView * view, QGraphicsScene * scene)
{
	view->addScene(scene);
	view->setInteractive(false);
	view->setMouseTracking(false);
	view->setBlocked(true);
}

void FlowNetworkAlgorithmWindow::scaleViews()
{
	QRectF networkRect = _network->childrenBoundingRect();
	QRectF viewRect = ui.mainNetworkView->geometry();
	float widthScale = viewRect.width() / networkRect.width();
	float heightScale = viewRect.height() / networkRect.height();
	_scaleFactor = std::min(widthScale, heightScale);
	if (_scaleFactor < 1.0f)
	{
		_scaleFactor -= 0.01f;	// odrobina przestrzeni przy ramce
		ui.mainNetworkView->scale(_scaleFactor, _scaleFactor);
		ui.residualNetworkView->scale(_scaleFactor, _scaleFactor);
	}
	_network->updateScale(_scaleFactor);
	_dx = std::max(
			static_cast<float>(networkRect.width() * 2.0),
			static_cast<float>(_network->sceneBoundingRect().width()));
	ui.mainNetworkView->setGraphImage(_network, QPointF(0, 0));
	ui.mainNetworkView->centerOn(_network);
	ui.residualNetworkView->centerOn(_network->pos().x() + _dx, _network->pos().y());
	_network->unselectAll();
}

void FlowNetworkAlgorithmWindow::setAlgorithm(FlowNetworkAlgorithm * algorithm)
{
	_algorithm = algorithm;
}

void FlowNetworkAlgorithmWindow::seNetwork(FlowNetwork * network)
{
	_network = network;
}

void FlowNetworkAlgorithmWindow::showEvent(QShowEvent * evt)
{
	scaleViews();
	QDialog::showEvent(evt);
}

void FlowNetworkAlgorithmWindow::createConnections()
{
	connect(ui.nextStepButton, SIGNAL(clicked()), this, SLOT(makeNextStep()));
	connect(ui.finishAlgorithmButton, SIGNAL(clicked()), this, SLOT(finish()));
}

void FlowNetworkAlgorithmWindow::updateConsole(QString const & message)
{
	_info.setInformation(_step, message);
	ui.progressConsole->insertPlainText(_info[_step]);
}

void FlowNetworkAlgorithmWindow::makeNextStep()
{
	if (_step % 3 == 0)
	{
		residualNewtork = _algorithm->makeResidualNetwork(_network);
		residualNewtork->updateScale(_scaleFactor);
		residualNewtork->unselectAll();
		QPointF residualPosition = QPointF(_network->pos().x() + _dx, _network->pos().y());
		ui.residualNetworkView->setGraphImage(residualNewtork, residualPosition);
		ui.residualNetworkView->centerOn(residualNewtork);
		QString message = Strings::Instance().get(FLOW_NETWORK_RESIDUAL_CREATED);
		updateConsole(message);
	}
	else if (_step % 3 == 1)
	{
		QList<int> path = _algorithm->findAugumentingPath();
		_algorithm->increaseFlow(path);
	}
	else
	{
		
	}
	_step++;
}

void FlowNetworkAlgorithmWindow::finish()
{
}