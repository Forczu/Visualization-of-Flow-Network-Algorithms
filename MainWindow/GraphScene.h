#pragma once
#include <QGraphicsScene>

class GraphScene : public QGraphicsScene
{
private:
	GraphScene(QRect const & rect);
public:
	~GraphScene();
	static GraphScene * getInstance();
};

