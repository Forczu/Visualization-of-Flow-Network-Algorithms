#pragma once
#include "FlowNetworkAlgorithm.h"
#include "FlowNetworkAlgorithmFactory.h"

namespace FordFulkerson
{
	/// <summary>
	/// Klasa reprezentuj¹ca algorytm Forda-Fulkersona. Zostaje zarejestrowana w fabryce algorytmów.
	/// </summary>
	class FordFulkersonAlgorithm : public FlowNetworkAlgorithm
	{
	public:
		int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;
		QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;
		void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase) override;
		QString resaidualNetworkFinishedMessage(int param) override;
		QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;
		void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
			QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;
	};

	inline FlowNetworkAlgorithm * getInstance() { return new FordFulkersonAlgorithm; }
	const QString name = "ford_fulkerson";
	const bool registered = FlowNetworkAlgorithmFactory::Instance().registerAlgorithm(name, getInstance);
}
