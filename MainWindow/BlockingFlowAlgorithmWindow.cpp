#include "BlockingFlowAlgorithmWindow.h"
#include "Strings.h"
#include "EdgeImage.h"
#include "VertexImage.h"
#include "DinicAlgorithm.h"
#include "GraphView.h"

BlockingFlowAlgorithmWindow::BlockingFlowAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent /*= 0*/)
: FlowNetworkAlgorithmWindow(network, algorithm, parent), _blockingFlowInProgress(false), _blockingFlow(nullptr), _blockingStep(0), _currentCapacity(0)
{
	// utworzenie dodatkowego okna dlareprezentacji przep³ywu blokuj¹cego
	blockingView = new GraphView(this);
	blockingView->setGeometry(ui.residualNetworkView->geometry());
	blockingView->setSizePolicy(ui.residualNetworkView->sizePolicy());
	blockingView->setMaximumSize(ui.residualNetworkView->maximumSize());
	blockingView->setFocusPolicy(ui.residualNetworkView->focusPolicy());
	blockingView->setHorizontalScrollBarPolicy(ui.residualNetworkView->horizontalScrollBarPolicy());
	blockingView->setVerticalScrollBarPolicy(ui.residualNetworkView->verticalScrollBarPolicy());
	configureView(blockingView);
	ui.gridLayout->addWidget(blockingView, 1, 2, 1, 1);
	auto blockingLabel = new QLabel(this);
	blockingLabel->setText(Strings::Instance().get(BLOCKING_FLOW));
	blockingLabel->setFont(ui.rightLabel->font());
	blockingLabel->setAlignment(ui.rightLabel->alignment());
	ui.gridLayout->addWidget(blockingLabel, 0, 2, 1, 1);
}

/// <summary>
/// Wykonanie nastêpnego kroku algorytmu z przep³ywem blokuj¹cym.
/// </summary>
void BlockingFlowAlgorithmWindow::makeNextStep()
{
	switch (_step % 5)
	{
	case 0:
		createResidualNetwork();
		break;
	case 1:
		createBlockingFlow();
		break;
	case 2:
		findAugumentingPath();
		break;
	case 3:
		increaseFlow();
		checkAlgorithmEnd();
		break;
	}
	if (!_blockingFlowInProgress)
		_step++;
	else
		_step = 2;
}

/// <summary>
/// Tworzy sieæ residualn¹. Je¿eli minimalna odleg³oœæ od Ÿród³a do ujœcia
/// w utworzonej sieci jest równo zeru, algorytm koñczy pracê.
/// </summary>
/// <returns></returns>
int BlockingFlowAlgorithmWindow::createResidualNetwork()
{
	int sourceTargetDistance = FlowNetworkAlgorithmWindow::createResidualNetwork();
	if (sourceTargetDistance == 0)
	{
		QString message = Strings::Instance().get(FLOW_NETWORK_ALGORITHM_FINISHED) + ' ' +
			Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_NOT_FOUND) + ' ' +
			Strings::Instance().get(FLOW_NETWORK_MAX_FLOW).arg(_algorithm->getMaxFlow());
		updateConsole(message);
		finishAlgorithm();
	}
	return sourceTargetDistance;
}

/// <summary>
/// Wyszukuje œcie¿kê powiêkszaj¹c¹ i zwiêksza przep³yw w sieci.
/// </summary>
void BlockingFlowAlgorithmWindow::findAugumentingPath()
{
	if (_blockingStep % 2 == 0)
		findAugumentingPathInBlockingFlow();
	else
		increaseFlowInResidaulNetwork();
	_blockingStep++;
}


/// <summary>
/// Zwiêksza przep³yw w przep³ywie sieci residualnej i aktualizauje przep³yw blokuj¹cy,
/// usuwa krawêdzie i wierzcho³ki je¿eli jest to potrzebne.
/// </summary>
void BlockingFlowAlgorithmWindow::increaseFlowInResidaulNetwork()
{
	_algorithm->increaseFlow(_residualNetwork, _currentBlockingPath, _currentCapacity);
	for (EdgeImage * edge : _currentBlockingPath)
	{
		edge->setCapacity(edge->getCapacity() - _currentCapacity);
	}
	bool verticesRemoved = _algorithm->removeNeedlessElements(_blockingFlow);
	QString message = Strings::Instance().get(RESIDUAL_NETWORK_FLOW_INCREASED);
	if (verticesRemoved)
	{
		if (dynamic_cast<DinicAlgorithm*>(_algorithm.data()) != NULL)
			message += ' ' + Strings::Instance().get(NULL_DEGREE_VERTICES_REMOVED);
		else
			message += ' ' + Strings::Instance().get(NULL_POTENTIAL_VERTICES_REMOVED);
	}
	updateConsole(message);
	if (!_blockingFlow->getSource()->isVisible() || !_blockingFlow->getTarget()->isVisible())
	{
		_blockingFlowInProgress = false;
		_blockingFlow->hide();
	}
}

void BlockingFlowAlgorithmWindow::showEvent(QShowEvent * evt)
{
	FlowNetworkAlgorithmWindow::showEvent(evt);
	blockingView->scale(_scaleFactor, _scaleFactor);
}

/// <summary>
/// Przeszukanie przep³ywu blokuj¹cego w celu znalezienia œcie¿ki powiêkszaj¹cej.
/// Je¿eli isnieje, dodaje j¹ i przep³yw do kontenerów.
/// </summary>
void BlockingFlowAlgorithmWindow::findAugumentingPathInBlockingFlow()
{
	_currentCapacity = 0;
	_currentBlockingPath = _algorithm->findAugumentingPath(_blockingFlow, _currentCapacity);
	if (_currentBlockingPath.size() != 0)
	{
		pushBlockingSet(_currentBlockingPath, _currentCapacity);
		QString message = _algorithm->augumentingPathFoundMessage(_currentBlockingPath, _currentCapacity);
		updateConsole(message);
	}
	else
	{
		_blockingFlowInProgress = false;
		QString message = Strings::Instance().get(FLOW_NETWORK_AUGUMENTING_PATH_NOT_FOUND)
			+ ' ' + Strings::Instance().get(BLOCKING_FLOW_FOUND);
		updateConsole(message);
		_blockingFlow->hide();
	}
}

void BlockingFlowAlgorithmWindow::pushBlockingSet(QList<EdgeImage*> const & path, int capacity)
{
	QList<EdgeImage*> residualPath;
	for (auto edge : path)
	{
		int from = edge->VertexFrom()->getId();
		int to = edge->VertexTo()->getId();
		residualPath.push_back(_residualNetwork->edgeAt(from, to));
	}
	_paths.push_back(residualPath);
	_capacities.push_back(capacity);
}

/// <summary>
/// Rozpoczyna szukanie nowego przep³ywu blokuj¹cego.
/// </summary>
void BlockingFlowAlgorithmWindow::createBlockingFlow()
{
	_blockingFlowInProgress = true;
	updateConsole(Strings::Instance().get(BLOCKING_FLOW_STARTED));
	QPointF residualPosition = _residualNetwork->pos();
	QPointF blockingPosition = QPointF(residualPosition.x() + _dx, residualPosition.y());
	if (_blockingFlow == nullptr)
	{
		_blockingFlow = _residualNetwork->clone();
		_blockingFlow->updateScale(_scaleFactor);
		blockingView->setGraphImage(_blockingFlow);
	}
	else
	{
		copyResidualNetworkAsBlockingFlow();
		_blockingFlow->updateScale(_scaleFactor);
		_blockingFlow->unselectAll();
	}
	_blockingFlow->setPos(blockingPosition);
	_blockingFlow->show();
	blockingView->centerOn(_blockingFlow);
	_blockingStep = 0;
}

/// <summary>
/// Utworzenie przep³ywu blokuj¹cego na podstawie aktualnej sieci residualnej.
/// </summary>
void BlockingFlowAlgorithmWindow::copyResidualNetworkAsBlockingFlow() const
{
	// usuniêcie starych krawêdzi
	auto oldEdges = _blockingFlow->getEdges();
	for (auto it = oldEdges.begin(); it != oldEdges.end(); ++it)
	{
		_blockingFlow->removeEdge(*it);
	}
	// skopiowanie nowych
	for (auto edge : _residualNetwork->getEdges())
	{
		_blockingFlow->addEdge(edge->VertexFrom()->getId(), edge->VertexTo()->getId(), edge->getCapacity(), EdgeType::StraightLine, edge->getFlow());
	}
	// ukryj niewidoczne wierzcho³ki
	for (auto vertex : _residualNetwork->getVertices())
	{
		VertexImage * blockingVertex = _blockingFlow->vertexAt(vertex->getId());
		if (!vertex->isVisible())
			blockingVertex->hide();
		else
			blockingVertex->show();
	}
}
