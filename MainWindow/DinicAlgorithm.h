#pragma once
#include "FlowNetworkAlgorithm.h"
#include "BlockingFlowAlgoritm.h"
#include "FlowNetworkAlgorithmFactory.h"

namespace Dinic
{
	/// <summary>
	/// Klasa reprezentuj¹ca algorytm Dinica. Zostaje zarejestrowana w fabryce algorytmów.
	/// </summary>
	class DinicAlgorithm : public FlowNetworkAlgorithm, public BlockingFlowAlgoritm
	{
	public:
		int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork) override;

		virtual QString resaidualNetworkFinishedMessage(int value) override;

		QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) override;
	private:

		void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
			QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) override;

		bool removeNeedlessElements(FlowNetwork * network) override;

	};
	inline FlowNetworkAlgorithm * getInstance() { return new DinicAlgorithm; }
	const QString name = "dinic";
	const bool registered = FlowNetworkAlgorithmFactory::Instance().registerAlgorithm(name, getInstance);
}