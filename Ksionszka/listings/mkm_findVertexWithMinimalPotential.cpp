VertexImage * MkmAlgorithm::findVertexWithMinimalPotential(FlowNetwork * network)
{
	float minimalPotential = std::numeric_limits<float>::infinity();
	VertexImage * chosenVertex = nullptr;
	for (PotentialMap::const_iterator it = _potentialMap.begin(); it != _potentialMap.end(); ++it)
	{
		float currentPotential = std::get<2>(*it);
		VertexImage * vertex = network->vertexAt(it.key());
		if (currentPotential != 0 && vertex->isVisible()
			&& !_rejectedVertices.contains(vertex) && currentPotential < minimalPotential)
		{
			chosenVertex = vertex;
			minimalPotential = currentPotential;
		}
	}
	return chosenVertex;
}