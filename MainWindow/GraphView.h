#pragma once

#include <QtWidgets/QGraphicsView>
#include <QGraphicsScene>
#include "QTimeLine"
#include "QWheelEvent"

#include "GraphScrollBar.h"
#include "Tools.h"
#include "VerticeImage.h"

class GraphView : public QGraphicsView
{
	Q_OBJECT

private:
	QGraphicsScene * _graphScene;

	bool _mouseClicked;

public:
	GraphView();
	GraphView(QWidget * widget);
	~GraphView();

	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

signals:
	void clicked(QPoint, QList<QGraphicsItem*>);
	void moved(QPoint);

};

