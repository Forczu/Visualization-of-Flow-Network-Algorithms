#pragma once
#include <QObject>
#include <QList>
class FlowNetwork;
class FlowNetworkAlgorithmWindow;
class EdgeImage;
class VertexImage;

class FlowNetworkAlgorithm : public QObject
{
	Q_OBJECT
protected:
	float _scaleFactor;
	int _currentMaxFlow;
	FlowNetworkAlgorithm();

public:
	virtual int makeResidualNetwork(FlowNetwork * network, FlowNetwork *& outResidaulNetwork);
	virtual QList<EdgeImage*> findAugumentingPath(FlowNetwork * residualNetwork, int & capacity);

	bool checkAugumentingPathExists(FlowNetwork * residualNetwork, VertexImage * source);

	virtual void increaseFlow(FlowNetwork *& network, QList<EdgeImage*> const & path, int increase);
	void setCurrentMaxFlow(int flow);
	inline int getMaxFlow() const { return _currentMaxFlow; }
	bool checkExistingPathsFromSource(FlowNetwork * network);
	bool checkExistingPathsToTarget(FlowNetwork * network);
	virtual QString resaidualNetworkFinishedMessage(int value = 0) = 0;
	virtual QString augumentingPathFoundMessage(QList<EdgeImage*> const & path, int capacity) = 0;
protected:
	virtual void addEdgeToPath(QList<EdgeImage*> & possibleEdges, EdgeImage * edge, VertexImage * currentVertex, VertexImage * source,
		QList<VertexImage*> const & visitedVertices, QList<VertexImage*> const & rejectedVertices) = 0;
	QList<EdgeImage*> findPathBetween(FlowNetwork * network, VertexImage * from, VertexImage * to);
};
