/// <summary>
/// Usuwa wszystkie nadmiarowe elementy grafu, które nie pojawią się w przepływie blokującym.
/// </summary>
/// <param name="residualNewtork">Sieć przepływowa.</param>
/// <returns></returns>
int BlockingFlowAlgoritm::removeRedundantElements(FlowNetwork *& residualNewtork)
{
	_sourceId = residualNewtork->getSourceId() - 1;
	_targetId = residualNewtork->getTargetId() - 1;
	createShortestPathsMatrix(residualNewtork);
	hideRedundantVertices(residualNewtork);
	removeRedundantEdges(residualNewtork);
	float sourceTargetDistance = _pathMatrix[_sourceId][_targetId];
	return sourceTargetDistance == std::numeric_limits<float>::infinity() ? 0 : sourceTargetDistance;
}