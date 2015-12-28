QList<EdgeImage*> FlowNetworkAlgorithm::findAugumentingPath(FlowNetwork * network, int & capacity)
{
	VertexImage * source = network->getSource();
	VertexImage * target = network->getTarget();
	QList<EdgeImage*> augumentingPath = augumentingPath = findPathBetween(network, source, target);
	if (augumentingPath.size() == 0)
		capacity = 0;
	else
	{
		auto it = std::min_element(augumentingPath.begin(), augumentingPath.end(), [&](EdgeImage * edge1, EdgeImage * edge2)
		{
			return edge1->getCapacity() < edge2->getCapacity();
		});
		_currentMaxFlow += (capacity = (*it)->getCapacity());
	}
	return augumentingPath;
}