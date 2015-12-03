#include "BlockingFlowAlgorithmWindow.h"
#include "Strings.h"
#include "EdgeImage.h"
#include "VertexImage.h"

BlockingFlowAlgorithmWindow::BlockingFlowAlgorithmWindow(FlowNetwork * network, FlowNetworkAlgorithm * algorithm, QWidget *parent /*= 0*/)
: FlowNetworkAlgorithmWindow(network, algorithm, parent), _blockingFlowInProgress(false), _blockingFlow(nullptr), _blockingStep(0), _currentCapacity(0)
{
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
		removeBlockingFlow();
		break;
	case 4:
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
	bool verticesRemoved = false;
	_algorithm->increaseFlow(_residualNetwork, _currentBlockingPath, _currentCapacity);
	for (EdgeImage * edge : _currentBlockingPath)
	{
		edge->setCapacity(edge->getCapacity() - _currentCapacity);
		if (edge->getCapacity() == 0)
			_blockingFlow->removeEdge(edge);
	}
	VertexImage * source = _blockingFlow->getSource();
	VertexImage * target = _blockingFlow->getTarget();
	VertexImage * vertex;
	for (auto item : _blockingFlow->getVertices())
	{
		vertex = item.second;
		if (vertex != source && vertex != target && (vertex->getOutDegree() == 0 || vertex->getInDegree() == 0))
		{
			vertex->hide();
			for (auto item : _blockingFlow->getEdges())
			{
				EdgeImage * edge = item.second;
				if (edge->VertexFrom() == vertex || edge->VertexTo() == vertex)
					_blockingFlow->removeEdge(edge);
			}
			if (!verticesRemoved)
				verticesRemoved = true;
		}
	}
	QString message = Strings::Instance().get(RESIDUAL_NETWORK_FLOW_INCREASED);
	if (verticesRemoved)
		message += ' ' + Strings::Instance().get(NULL_DEGREE_VERTICES_REMOVED);
	updateConsole(message);
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
	ui.leftLabel->setText(Strings::Instance().get(RESIDUAL_NETWORK));
	ui.rightLabel->setText(Strings::Instance().get(BLOCKING_FLOW));
	QPointF networkPosition = _network->pos();
	QPointF residualPosition = _residualNetwork->pos();
	_network->hide();
	_residualNetwork->setPos(networkPosition);
	if (_blockingFlow == nullptr)
	{
		_blockingFlow = _residualNetwork->clone();
		_scene->addItem(_blockingFlow);
	}
	else
	{
		copyResidualNetworkAsBlockingFlow();
		_blockingFlow->show();
		_blockingFlow->unselectAll();
	}
	_blockingFlow->updateScale(_scaleFactor);
	_blockingFlow->setPos(residualPosition);
	_blockingStep = 0;
}

/// <summary>
/// Koñczy przeszukianie przep³ywu blokuj¹cego i usuwa go z podgl¹du.
/// </summary>
void BlockingFlowAlgorithmWindow::removeBlockingFlow()
{
	// przywróæ podgl¹d do poprzedniego stanu
	ui.leftLabel->setText(Strings::Instance().get(FLOW_NETWORK));
	ui.rightLabel->setText(Strings::Instance().get(RESIDUAL_NETWORK));
	QPointF residualPosition = _blockingFlow->pos();
	_network->show();
	_residualNetwork->setPos(residualPosition);
	_blockingFlow->hide();
	QString message = Strings::Instance().get(FLOW_NETWORK_RESTORED);
	updateConsole(message);
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
		_blockingFlow->removeEdge((*it).second);
	}
	// skopiowanie nowych
	for (auto item : _residualNetwork->getEdges())
	{
		EdgeImage * edge = item.second;
		_blockingFlow->addEdge(edge->VertexFrom()->getId(), edge->VertexTo()->getId(), edge->getCapacity(), EdgeType::StraightLine, edge->getFlow());
	}
	// ukryj niewidoczne wierzcho³ki
	for (auto item : _residualNetwork->getVertices())
	{
		VertexImage * vertex = item.second;
		VertexImage * blockingVertex = _blockingFlow->vertexAt(vertex->getId());
		if (!vertex->isVisible())
			blockingVertex->hide();
		else
			blockingVertex->show();
	}
}
