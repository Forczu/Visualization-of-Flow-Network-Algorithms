#include "FlowNetworkAlgorithmWindow.h"
#include <math.h>
#include <chrono>
#include <thread>
#include "IAlgorithm.h"
#include "GraphScene.h"
#include "Strings.h"
#include "EdgeImage.h"
#include "VertexImage.h"

FlowNetworkAlgorithmWindow::FlowNetworkAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent)
: QDialog(parent), _algorithm(algorithm), _network(network), _step(0), _residualNetwork(nullptr), _finished(false)
{
	ui.setupUi(this);
	createConnections();
	_scene = GraphScene::getInstance();
	configureView(ui.mainNetworkView);
	configureView(ui.residualNetworkView);
}

FlowNetworkAlgorithmWindow::~FlowNetworkAlgorithmWindow()
{
	deleteDialog();
}

void FlowNetworkAlgorithmWindow::setSceneForViews(QGraphicsScene * scene)
{
	scene->setParent(this);
	ui.mainNetworkView->addScene(scene);
	ui.residualNetworkView->addScene(scene);
}

void FlowNetworkAlgorithmWindow::configureView(GraphView * view)
{
	view->addScene(_scene);
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
	ui.mainNetworkView->setGraphImage(_network);
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

void FlowNetworkAlgorithmWindow::closeEvent(QCloseEvent * evt)
{
	deleteDialog();
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
	ui.progressConsole->verticalScrollBar()->setValue(ui.progressConsole->verticalScrollBar()->maximum());
}

void FlowNetworkAlgorithmWindow::makeNextStep()
{
	static QList<EdgeImage*> path;
	static int capacity;
	if (_step % 3 == 0)
	{
		_network->unselectAll();
		delete _residualNetwork;
		_residualNetwork = _algorithm->makeResidualNetwork(_network);
		_residualNetwork->updateScale(_scaleFactor);
		_residualNetwork->unselectAll();
		QPointF residualPosition = QPointF(_network->pos().x() + _dx, _network->pos().y());
		ui.residualNetworkView->setGraphImage(_residualNetwork, residualPosition);
		ui.residualNetworkView->centerOn(_residualNetwork);
		QString message = Strings::Instance().get(FLOW_NETWORK_RESIDUAL_CREATED);
		updateConsole(message);
	}
	else if (_step % 3 == 1)
	{
		path = _algorithm->findAugumentingPath(_residualNetwork, capacity);
		if (capacity != 0)
		{
			QString numbers;
			numbers.push_back(QString::number(path.first()->VertexFrom()->getId()) + ' ');
			for (EdgeImage * edge : path)
			{
				edge->setSelected(true);
				numbers.push_back(QString::number(edge->VertexTo()->getId()) + ' ');
			}
			QString message = Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_FOUND)
				.arg(numbers).arg(capacity);
			updateConsole(message);
			_finished = true;
		}
		else
		{
			QString message = Strings::Instance().get(FLOW_NETWORK_ALGORITHM_FINISHED) + ' ' +
				Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_NOT_FOUND) + ' ' +
				Strings::Instance().get(FLOW_NETWORK_MAX_FLOW).arg(_algorithm->getMaxFlow());
			updateConsole(message);
			ui.nextStepButton->setEnabled(false);
			ui.finishAlgorithmButton->setEnabled(false);
		}
	}
	else
	{
		int oldFlow;
		EdgeImage * networkEdge;
		for (EdgeImage * edge : path)
		{
			int vertexFromId = edge->VertexFrom()->getId();
			int vertexToId = edge->VertexTo()->getId();
			networkEdge = _network->edgeAt(vertexFromId, vertexToId);

			// je¿eli krawêdŸ nie istnieje w prawdziwej sieci, nale¿y utworzyæ przep³yw zwrotny
			if (networkEdge == nullptr)
			{
				networkEdge = _network->edgeAt(vertexToId, vertexFromId);
				oldFlow = networkEdge->getFlow();
				networkEdge->setFlow(oldFlow - capacity);
			}
			// je¿eli krawêdŸ istnieje, ale posiada s¹siada, nale¿y zmniejszyæ jego przep³yw
			else if (networkEdge != nullptr && networkEdge->hasNeighbor())
			{
				oldFlow = networkEdge->getFlow();
				EdgeImage * neighbourEdge = _network->edgeAt(edge->VertexTo()->getId(), edge->VertexFrom()->getId());
				int currentNeighborFlow = neighbourEdge->getFlow();
				networkEdge->setFlow(std::max(capacity - currentNeighborFlow, 0));
				neighbourEdge->setFlow(std::max(currentNeighborFlow - capacity, 0));
			}
			// istnieje, ale nie posiada s¹siada, zwyk³e zwiêkszenie
			else
			{
				oldFlow = networkEdge->getFlow();
				networkEdge->setFlow(oldFlow + capacity);
			}
			edge->setSelected(false);
			networkEdge->setSelected(true);
		}
		QString message = Strings::Instance().get(FLOW_NETWORK_FLOW_INCREASED)
			.arg(_algorithm->getMaxFlow());
		updateConsole(message);
		VertexImage * source = _network->vertexAt(_network->getSource());
		VertexImage * target = _network->vertexAt(_network->getTarget());
		bool finished1 = true;
		for (auto item : _network->getEdges())
		{
			networkEdge = item.second;
			if (networkEdge->VertexFrom() == source)
				if (networkEdge->getFlow() == networkEdge->getCapacity())
					continue;
				else
				{
					finished1 = false;
					break;
				}
		}
		if (finished1)
		{
			QString message = Strings::Instance().get(FLOW_NETWORK_ALGORITHM_FINISHED) + ' ' +
				Strings::Instance().get(FLOW_NETWORK_FULL_SOURCE_CAPACITY) + ' ' +
				Strings::Instance().get(FLOW_NETWORK_MAX_FLOW).arg(_algorithm->getMaxFlow());
			updateConsole(message);
			ui.nextStepButton->setEnabled(false);
			ui.finishAlgorithmButton->setEnabled(false);
			_finished = true;
			emit endAlgorithm();
			return;
		}
		bool finished2 = true;
		for (auto item : _network->getEdges())
		{
			networkEdge = item.second;
			if (networkEdge->VertexTo() == target)
			{
				if (networkEdge->getFlow() == networkEdge->getCapacity())
					continue;
				else
				{
					finished2 = false;
					break;
				}
			}
		}
		if (finished2)
		{
			QString message = Strings::Instance().get(FLOW_NETWORK_ALGORITHM_FINISHED) + ' ' +
				Strings::Instance().get(FLOW_NETWORK_FULL_TARGET_CAPACITY) + ' ' +
				Strings::Instance().get(FLOW_NETWORK_MAX_FLOW).arg(_algorithm->getMaxFlow());
			updateConsole(message);
			ui.nextStepButton->setEnabled(false);
			ui.finishAlgorithmButton->setEnabled(false);
			_finished = true;
			emit endAlgorithm();
			return;
		}
	}
	_step++;
	ui.mainNetworkView->updateGeometry();
	ui.residualNetworkView->updateGeometry();
}

void FlowNetworkAlgorithmWindow::stopTimer()
{
	_timer->stop();
	_network->unselectAll();
}

void FlowNetworkAlgorithmWindow::finish()
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(makeNextStep()));
	connect(this, SIGNAL(endAlgorithm()), this, SLOT(stopTimer()));
	_timer->start(500);
}

void FlowNetworkAlgorithmWindow::deleteDialog()
{
	if (_residualNetwork)
		delete _residualNetwork;
	if (_algorithm)
		delete _algorithm;
}
