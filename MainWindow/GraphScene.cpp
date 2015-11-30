#include "GraphScene.h"


GraphScene::GraphScene(QRect const & rect) : QGraphicsScene(rect)
{
}


GraphScene::~GraphScene()
{
}

GraphScene * GraphScene::getInstance()
{
	return new GraphScene(QRect(POS_X, POS_Y, WIDTH, HEIGHT));
}
