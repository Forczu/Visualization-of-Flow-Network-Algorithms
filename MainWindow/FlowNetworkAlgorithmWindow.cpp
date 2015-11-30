#include "FlowNetworkAlgorithmWindow.h"
#include "GraphScene.h"
#include "Strings.h"
#include "EdgeImage.h"
#include "VertexImage.h"

FlowNetworkAlgorithmWindow::FlowNetworkAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent)
: QDialog(parent), _algorithm(algorithm), _network(network), _step(0), _residualNetwork(nullptr), _capacity(0)
{
	ui.setupUi(this);
	createConnections();
	createScene();
	configureView(ui.mainNetworkView);
	configureView(ui.residualNetworkView);
	_started =_finished = false;
}

void FlowNetworkAlgorithmWindow::createScene()
{
	_scene = GraphScene::getInstance();
	QRectF rect = _scene->sceneRect();
	rect.setWidth(rect.width() * 3);
	_scene->setSceneRect(rect);
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

void FlowNetworkAlgorithmWindow::configureView(GraphView * view) const
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
	int windowWidth = ui.mainNetworkView->viewport()->width();
	_dx = _scene->width() / 2.0f;
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

int FlowNetworkAlgorithmWindow::getCurrentFlowToTarget(FlowNetwork* network)
{
	int maxFlow = 0;
	VertexImage * target = network->vertexAt(network->getTarget());
	EdgeImage * edge;
	for (auto item : network->getEdges())
	{
		edge = item.second;
		if (edge->VertexTo() == target)
		{
			maxFlow += edge->getFlow();
		}
	}
	return maxFlow;
}

void FlowNetworkAlgorithmWindow::createConnections() const
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
	if (_step % 3 == 0)
	{
		_network->unselectAll();
		if (!_started)
		{
			_algorithm->setCurrentMaxFlow(getCurrentFlowToTarget(_network));
			_residualNetwork = _network->clone();
			_started = true;
		}
		_algorithm->makeResidualNetwork(_network, _residualNetwork);
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
		_path = _algorithm->findAugumentingPath(_residualNetwork, _capacity);
		if (_capacity != 0)
		{
			QString numbers;
			numbers.push_back(QString::number(_path.first()->VertexFrom()->getId()) + ' ');
			for (EdgeImage * edge : _path)
			{
				edge->setSelected(true);
				numbers.push_back(QString::number(edge->VertexTo()->getId()) + ' ');
			}
			QString message = Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_FOUND)
				.arg(numbers).arg(_capacity);
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
		for (EdgeImage * edge : _path)
		{
			int vertexFromId = edge->VertexFrom()->getId();
			int vertexToId = edge->VertexTo()->getId();
			networkEdge = _network->edgeAt(vertexFromId, vertexToId);

			// je¿eli krawêdŸ nie istnieje w prawdziwej sieci, nale¿y utworzyæ przep³yw zwrotny
			if (networkEdge == nullptr)
			{
				networkEdge = _network->edgeAt(vertexToId, vertexFromId);
				oldFlow = networkEdge->getFlow();
				networkEdge->setFlow(oldFlow - _capacity);
			}
			// je¿eli krawêdŸ istnieje, ale posiada s¹siada, nale¿y zmniejszyæ jego przep³yw
			else if (networkEdge != nullptr && networkEdge->hasNeighbor())
			{
				oldFlow = networkEdge->getFlow();
				EdgeImage * neighbourEdge = _network->edgeAt(edge->VertexTo()->getId(), edge->VertexFrom()->getId());
				int currentNeighborFlow = neighbourEdge->getFlow();
				networkEdge->setFlow(std::max(_capacity - currentNeighborFlow, 0));
				neighbourEdge->setFlow(std::max(currentNeighborFlow - _capacity, 0));
			}
			// istnieje, ale nie posiada s¹siada, zwyk³e zwiêkszenie
			else
			{
				oldFlow = networkEdge->getFlow();
				networkEdge->setFlow(oldFlow + _capacity);
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

void FlowNetworkAlgorithmWindow::stopTimer() const
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

void FlowNetworkAlgorithmWindow::deleteDialog() const
{
	if (_algorithm)
		delete _algorithm;
}
