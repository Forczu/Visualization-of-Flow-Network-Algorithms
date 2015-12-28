QList<EdgeImage*> MkmAlgorithm::findAugumentingPath(FlowNetwork * network, int & capacity)
{
	QList<EdgeImage*> pathToTarget, pathToSource;
	do 
	{
		pathToTarget.clear();
		pathToSource.clear();
		VertexImage * vertex = findVertexWithMinimalPotential(network);
		if (vertex == nullptr)
			break;
		pathToTarget = sendUnitsToTarget(network, vertex->getId());
		pathToSource = sendUnitsToSource(network, vertex->getId());
		bool succeeded = pathToSource.size() != 0 && pathToTarget.size() != 0;
		if (!succeeded)
			_rejectedVertices.push_back(vertex);
		else
		{
			capacity = std::get<2>(_potentialMap[vertex->getId()]);
			_currentMaxFlow += capacity;
			vertex->setSelected(true);
			network->vertexAt(vertex->getId())->setSelected(true);
		}
	} while (pathToSource.size() == 0 || pathToTarget.size() == 0);
	return pathToSource + pathToTarget;
}