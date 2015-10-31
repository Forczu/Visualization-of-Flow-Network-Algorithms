#include "GraphScene.h"


GraphScene::GraphScene(QRect const & rect) : QGraphicsScene(rect)
{
}


GraphScene::~GraphScene()
{
}

GraphScene * GraphScene::getInstance()
{
	return new GraphScene(QRect(-2000, -2000, 4000, 4000));
}
