#include "FlowNetworkAlgorithmWindow.h"
#include "GraphScene.h"
#include "Strings.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "DinicAlgorithm.h"
#include "GraphSerializer.h"
#include "QFileDialog"

FlowNetworkAlgorithmWindow::FlowNetworkAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent)
: QDialog(parent), _algorithm(algorithm), _network(network), _step(0), _residualNetwork(nullptr)
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
	VertexImage * target = network->vertexAt(network->getTargetId());
	EdgeImage * edge;
	for (auto edge : network->getEdges())
	{
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
	connect(ui.saveResultButton, SIGNAL(clicked()), this, SLOT(saveResult()));
}

void FlowNetworkAlgorithmWindow::updateConsole(QString const & message)
{
	_info.setInformation(_step, message);
	ui.progressConsole->insertPlainText(_info[_step]);
	ui.progressConsole->verticalScrollBar()->setValue(ui.progressConsole->verticalScrollBar()->maximum());
}

/// <summary>
/// Wykonanie nastêpnego kroku algorytmu.
/// </summary>
void FlowNetworkAlgorithmWindow::makeNextStep()
{
	switch (_step % 3)
	{
	case 0:
		createResidualNetwork();
		break;
	case 1:
		findAugumentingPath();
		break;
	case 2:
		increaseFlow();
		checkAlgorithmEnd();
		break;
	}
	_step++;
	ui.mainNetworkView->updateGeometry();
	ui.residualNetworkView->updateGeometry();
}

/// <summary>
/// Zwiêksza przep³yw w sieci przep³ywaj¹cej o dodatkow¹ wartoœæ w znalezionej œcie¿ce.
/// </summary>
void FlowNetworkAlgorithmWindow::increaseFlow()
{
	for (int i = 0; i < _capacities.size(); ++i)
	{
		_algorithm->increaseFlow(_network, _paths[i], _capacities[i]);
	}
	clearSets();
	QString message = Strings::Instance().get(FLOW_NETWORK_FLOW_INCREASED)
		.arg(_algorithm->getMaxFlow());
	updateConsole(message);
}

/// <summary>
/// Sprawdza, czy algorytm zakoñczy³ swoj¹ pracê. Jeœli tak, wypisuje o tym komunikat.
/// </summary>
void FlowNetworkAlgorithmWindow::checkAlgorithmEnd()
{
	bool pathFromSourceExists = _algorithm->checkExistingPathsFromSource(_network);
	bool pathToTargetExists = _algorithm->checkExistingPathsToTarget(_network);
	if (!pathFromSourceExists || !pathToTargetExists)
	{
		QString message = Strings::Instance().get(FLOW_NETWORK_ALGORITHM_FINISHED);
		if (!pathFromSourceExists)
			message += ' ' + Strings::Instance().get(FLOW_NETWORK_FULL_SOURCE_CAPACITY);
		if (!pathToTargetExists)
			message += ' ' + Strings::Instance().get(FLOW_NETWORK_FULL_TARGET_CAPACITY);
		message += ' ' + Strings::Instance().get(FLOW_NETWORK_MAX_FLOW).arg(_algorithm->getMaxFlow());
		updateConsole(message);
		finishAlgorithm();
	}
}

/// <summary>
/// Znajduje now¹ œcie¿kê powiêkszaj¹c¹ w sieci residualnej.
/// </summary>
void FlowNetworkAlgorithmWindow::findAugumentingPath()
{
	int capacity = 0;
	auto path = _algorithm->findAugumentingPath(_residualNetwork, capacity);
	if (capacity != 0)
	{
		QString message = _algorithm->augumentingPathFoundMessage(path, capacity);
		pushNewSet(path, capacity);
		updateConsole(message);
	}
	else
	{
		QString message = Strings::Instance().get(FLOW_NETWORK_ALGORITHM_FINISHED) + ' ' +
			Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_NOT_FOUND) + ' ' +
			Strings::Instance().get(FLOW_NETWORK_MAX_FLOW).arg(_algorithm->getMaxFlow());
		updateConsole(message);
		finishAlgorithm();
	}
}

/// <summary>
/// Koñczy pracê algorytmu i przygotowuje okno na zakoñczenie.
/// </summary>
void FlowNetworkAlgorithmWindow::finishAlgorithm()
{
	ui.nextStepButton->setEnabled(false);
	ui.finishAlgorithmButton->setEnabled(false);
	ui.saveResultButton->setEnabled(true);
	_finished = true;
	emit endAlgorithm();
}

/// <summary>
/// Utworzenie nowej sieci residualnej na podstawie sieci wejœciowej.
/// </summary>
int FlowNetworkAlgorithmWindow::createResidualNetwork()
{
	_network->unselectAll();
	// je¿eli to pierwszy krok, sklonuj aktualn¹ sieæ
	if (!_started)
	{
		_algorithm->setCurrentMaxFlow(getCurrentFlowToTarget(_network));
		_residualNetwork = _network->clone();
		_started = true;
	}
	// utworzenie sieci residualnej i umieszczenie jej na scenie
	auto value = _algorithm->makeResidualNetwork(_network, _residualNetwork);
	_residualNetwork->updateScale(_scaleFactor);
	_residualNetwork->unselectAll();
	QPointF residualPosition = QPointF(_network->pos().x() + _dx, _network->pos().y());
	ui.residualNetworkView->setGraphImage(_residualNetwork, residualPosition);
	ui.residualNetworkView->centerOn(_residualNetwork);
	QString message = _algorithm->resaidualNetworkFinishedMessage(value);
	updateConsole(message);
	return value;
}

void FlowNetworkAlgorithmWindow::stopTimer() const
{
	_timer->stop();
	_network->unselectAll();
}

void FlowNetworkAlgorithmWindow::saveResult()
{
	std::string fileName = QFileDialog::getSaveFileName(this,
		Strings::Instance().get(SAVE_GRAPH_TO_FILE), QString(), tr("XML File (*.xml)")).toStdString();
	if (fileName.empty())
		return;
	GraphSerializer serializer;
	serializer.serialize(*_network, fileName);
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

void FlowNetworkAlgorithmWindow::pushNewSet(QList<EdgeImage*> const & path, int capacity)
{
	_paths.push_back(path);
	_capacities.push_back(capacity);
}

void FlowNetworkAlgorithmWindow::clearSets()
{
	_paths.clear();
	_capacities.clear();
}
