#pragma once
#include <QMap>
#include "FlowNetworkAlgorithm.h"
#include "BlockingFlowAlgoritm.h"
#include "FlowNetworkAlgorithmFactory.h"

namespace Mkm
{
	class MkmAlgorithm : public FlowNetworkAlgorithm, public BlockingFlowAlgoritm
	{
		typedef QMap<int, std::tuple<float, float, float>> PotentialMap;
		PotentialMap _potentialMap;
		QList<VertexImage*> _rejectedVertices;
	public:
		int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& residualNewtork) override;

		QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity) override;

		QString resaidualNetworkFinishedMessage(int value = 0) override;

		QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;

		void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source, QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;

		bool removeNeedlessElements(FlowNetwork * network) override;
	private:
		void calculateVertexPotentials(FlowNetwork * network);

		VertexImage * findVertexWithMinimalPotential(FlowNetwork * network);

		QList<EdgeImage*> sendUnitsToTarget(FlowNetwork * network, int vertexId);

		QList<EdgeImage*> sendUnitsToSource(FlowNetwork * network, int vertexId);

	};

	inline FlowNetworkAlgorithm * getInstance() { return new MkmAlgorithm; }
	const QString name = "mkm";
	const bool registered = FlowNetworkAlgorithmFactory::Instance().registerAlgorithm(name, getInstance);
}