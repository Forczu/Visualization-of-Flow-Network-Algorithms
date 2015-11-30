#pragma once
#include <QGraphicsScene>

class GraphScene : public QGraphicsScene
{
	static const int POS_X = -2000;
	static const int POS_Y = -2000;
	static const int WIDTH = 4000;
	static const int HEIGHT = 4000;

	explicit GraphScene(QRect const & rect);
public:
	~GraphScene();
	static GraphScene * getInstance();
};

