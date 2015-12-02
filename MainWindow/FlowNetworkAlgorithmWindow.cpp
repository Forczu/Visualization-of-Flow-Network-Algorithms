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

/// <summary>
/// Wykonanie nast�pnego kroku algorytmu.
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
/// Zwi�ksza przep�yw w sieci przep�ywaj�cej o dodatkow� warto�� w znalezionej �cie�ce.
/// </summary>
void FlowNetworkAlgorithmWindow::increaseFlow()
{
	_algorithm->increaseFlow(_network, _path, _capacity);
	QString message = Strings::Instance().get(FLOW_NETWORK_FLOW_INCREASED)
		.arg(_algorithm->getMaxFlow());
	updateConsole(message);
}

/// <summary>
/// Sprawdza, czy algorytm zako�czy� swoj� prac�. Je�li tak, wypisuje o tym komunikat.
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
/// Znajduje now� �cie�k� powi�kszaj�c� w sieci residualnej.
/// </summary>
void FlowNetworkAlgorithmWindow::findAugumentingPath()
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
/// Ko�czy prac� algorytmu i przygotowuje okno na zako�czenie.
/// </summary>
void FlowNetworkAlgorithmWindow::finishAlgorithm()
{
	ui.nextStepButton->setEnabled(false);
	ui.finishAlgorithmButton->setEnabled(false);
	_finished = true;
	emit endAlgorithm();
}

/// <summary>
/// Utworzenie nowej sieci residualnej na podstawie sieci wej�ciowej.
/// </summary>
void FlowNetworkAlgorithmWindow::createResidualNetwork()
{
	_network->unselectAll();
	// je�eli to pierwszy krok, sklonuj aktualn� sie�
	if (!_started)
	{
		_algorithm->setCurrentMaxFlow(getCurrentFlowToTarget(_network));
		_residualNetwork = _network->clone();
		_started = true;
	}
	// utworzenie sieci residualnej i umieszczenie jej na scenie
	_algorithm->makeResidualNetwork(_network, _residualNetwork);
	_residualNetwork->updateScale(_scaleFactor);
	_residualNetwork->unselectAll();
	QPointF residualPosition = QPointF(_network->pos().x() + _dx, _network->pos().y());
	ui.residualNetworkView->setGraphImage(_residualNetwork, residualPosition);
	ui.residualNetworkView->centerOn(_residualNetwork);
	printMessage(FLOW_NETWORK_RESIDUAL_CREATED);
}

void FlowNetworkAlgorithmWindow::stopTimer() const
{
	_timer->stop();
	_network->unselectAll();
}

void FlowNetworkAlgorithmWindow::printMessage(const char * key)
{
	QString message = Strings::Instance().get(key);
	updateConsole(message);
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
