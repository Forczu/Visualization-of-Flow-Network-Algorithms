#include "GraphShapeDialog.h"
#include "Config.h"
#include "VertexImage.h"
#include "StraightEdgeImage.h"
#include "ArrowHeadImage.h"
#include "Edge.h"

GraphShapeDialog::GraphShapeDialog(GraphConfig * config, QWidget *parent)
: QDialog(parent), _config(config)
{
	ui.setupUi(this);
	ui.applyButton->setEnabled(false);
	// utworzenie kopii zapasowej ustawieñ
	_backupConfig = _config->clone();
	initVertexTab();
	initEdgeTab();
	createConnections();
}

GraphShapeDialog::~GraphShapeDialog()
{
	clearPreview(ui.vertexPreview);
	clearPreview(ui.edgePreview);
	delete _backupConfig;
	delete _v1;
	delete _v2;
	delete _vertex, _edge;
}

void GraphShapeDialog::clearPreview(QGraphicsView * const preview)
{
	QGraphicsScene * scene = preview->scene();
	QList<QGraphicsItem*> items = scene->items();
	std::for_each(items.begin(), items.end(), [&](QGraphicsItem * item)
	{
		scene->removeItem(item);
		delete item;
	});
	delete scene;
}

void GraphShapeDialog::initVertexTab()
{
	QPoint startPoint = ui.vertexPreview->pos();
	int sceneWidth = ui.vertexPreview->width();
	int sceneHeight = ui.vertexPreview->height();

	QGraphicsScene * graphScene = new QGraphicsScene(startPoint.x(), startPoint.y(), sceneWidth, sceneHeight);
	ui.vertexPreview->setScene(graphScene);

	auto vertexConext = _config->NormalVertexContext();
	_verticePreview = new VertexImage(vertexConext->clone());
	_verticePreview->setPos(startPoint.x() + sceneWidth / 2.0f, startPoint.y() + sceneHeight / 2.0f);
	_verticePreview->setFlag(QGraphicsItem::ItemIsMovable, false);
	_verticePreview->setVertex(_vertex = new Vertex(1));
	graphScene->addItem(_verticePreview);

	ui.vertexSizeSlider->setValue(vertexConext->Size());
	ui.strokeSizeSlider->setValue(vertexConext->StrokeSize());
	ui.vertexColorRedBox->setValue(vertexConext->Color().red());
	ui.vertexColorGreenBox->setValue(vertexConext->Color().green());
	ui.vertexColorBlueBox->setValue(vertexConext->Color().blue());
	ui.strokeColorRedBox->setValue(vertexConext->StrokeColor().red());
	ui.strokeColorGreenBox->setValue(vertexConext->StrokeColor().green());
	ui.strokeColorBlueBox->setValue(vertexConext->StrokeColor().blue());
}

void GraphShapeDialog::initEdgeTab()
{
	QPoint sceneStart = ui.edgePreview->pos();
	int sceneWidth = ui.edgePreview->width();
	int sceneHeight = ui.edgePreview->height();
	QPoint startPoint = QPoint(sceneStart.x() + 10, sceneStart.y() + 10);
	QPoint endPoint = QPoint(startPoint.x() + sceneWidth - 10, startPoint.y() + sceneHeight - 10);

	QGraphicsScene * graphScene = new QGraphicsScene(sceneStart.x(), sceneStart.y(), sceneWidth, sceneHeight);
	ui.edgePreview->setScene(graphScene);
	auto vertexConext = _config->NormalVertexContext();
	auto edgeContext = _config->NormalEdgeContext();
	_v1 = new VertexImage(vertexConext->clone());
	_v2 = new VertexImage(vertexConext->clone());
	_v1->setPos(startPoint);
	_v2->setPos(endPoint);
	int size = vertexConext->Size();
	_edgePreview = new StraightEdgeImage(_edge = new Edge(), _v1, _v2, edgeContext);
	graphScene->addItem(_edgePreview);

	int dx = -(endPoint.x() - startPoint.x());
	int dy = -(endPoint.y() - startPoint.y());
	float angle = std::atan2(dy, dx);
	size = edgeContext->Size();
	_arrow = new ArrowHeadImage(_edgePreview, 4.0f * size, 6.0f * size, angle * 180.0f / M_PI + 90, true);
	_arrow->setPos(endPoint);
	_arrow->setZValue(3);
	_arrow->setColor(edgeContext->Color());
	_arrow->setParentItem(_edgePreview);

	ui.edgeSizeSlider->setValue(edgeContext->Size());
	ui.edgeColorRedBox->setValue(edgeContext->Color().red());
	ui.edgeColorGreenBox->setValue(edgeContext->Color().green());
	ui.edgeColorBlueBox->setValue(edgeContext->Color().blue());

	ui.edgePreview->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void GraphShapeDialog::updateVertexPreview()
{
	_verticePreview->Context(_config->NormalVertexContext());
	ui.vertexPreview->viewport()->update();
	enableApplyButton();
}

void GraphShapeDialog::updateEdgePreview()
{
	_edgePreview->Context(_config->NormalEdgeContext());
	ui.edgePreview->viewport()->update();
	enableApplyButton();
}

void GraphShapeDialog::createConnections()
{
	connect(ui.vertexSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(vertexSizeChanged(int)));
	connect(ui.strokeSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(strokeSizeChanged(int)));
	connect(ui.vertexColorRedBox, SIGNAL(valueChanged(int)), this, SLOT(vertexColorRedChanged(int)));
	connect(ui.vertexColorGreenBox, SIGNAL(valueChanged(int)), this, SLOT(vertexColorBlueChanged(int)));
	connect(ui.vertexColorBlueBox, SIGNAL(valueChanged(int)), this, SLOT(vertexColorGreenChanged(int)));
	connect(ui.strokeColorRedBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorRedChanged(int)));
	connect(ui.strokeColorGreenBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorBlueChanged(int)));
	connect(ui.strokeColorBlueBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorGreenChanged(int)));

	connect(ui.edgeSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(edgeSizeChanged(int)));
	connect(ui.edgeColorRedBox, SIGNAL(valueChanged(int)), this, SLOT(edgeColorRedChanged(int)));
	connect(ui.edgeColorBlueBox, SIGNAL(valueChanged(int)), this, SLOT(edgeColorBlueChanged(int)));
	connect(ui.edgeColorGreenBox, SIGNAL(valueChanged(int)), this, SLOT(edgeColorGreenChanged(int)));

	connect(ui.okButton, SIGNAL(clicked(bool)), this, SLOT(acceptChanges()));
	connect(ui.cancelButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));
	connect(ui.applyButton, SIGNAL(clicked(bool)), this, SLOT(applyChanges()));
}

void GraphShapeDialog::enableApplyButton()
{
	if (!ui.applyButton->isEnabled())
		ui.applyButton->setEnabled(true);
}

void GraphShapeDialog::vertexSizeChanged(int val)
{
	_config->NormalVertexContext()->Size(val);
	_config->SelectedVertexContext()->Size(val);
	updateVertexPreview();
}

void GraphShapeDialog::strokeSizeChanged(int val)
{
	_config->NormalVertexContext()->StrokeSize(val);
	_config->SelectedVertexContext()->StrokeSize(val);
	updateVertexPreview();
}

void GraphShapeDialog::vertexColorRedChanged(int val)
{
	QColor color = _config->NormalVertexContext()->Color();
	color.setRed(val);
	_config->NormalVertexContext()->Color(color);
	updateVertexPreview();
}

void GraphShapeDialog::vertexColorGreenChanged(int val)
{
	QColor color = _config->NormalVertexContext()->Color();
	color.setGreen(val);
	_config->NormalVertexContext()->Color(color);
	updateVertexPreview();
}

void GraphShapeDialog::vertexColorBlueChanged(int val)
{
	QColor color = _config->NormalVertexContext()->Color();
	color.setBlue(val);
	_config->NormalVertexContext()->Color(color);
	updateVertexPreview();
}

void GraphShapeDialog::strokeColorRedChanged(int val)
{
	QColor color = _config->NormalVertexContext()->StrokeColor();
	color.setRed(val);
	_config->NormalVertexContext()->StrokeColor(color);
	updateVertexPreview();
}

void GraphShapeDialog::strokeColorGreenChanged(int val)
{
	QColor color = _config->NormalVertexContext()->StrokeColor();
	color.setGreen(val);
	_config->NormalVertexContext()->StrokeColor(color);
	updateVertexPreview();
}

void GraphShapeDialog::strokeColorBlueChanged(int val)
{
	QColor color = _config->NormalVertexContext()->StrokeColor();
	color.setBlue(val);
	_config->NormalVertexContext()->StrokeColor(color);
	updateVertexPreview();
}

void GraphShapeDialog::edgeSizeChanged(int val)
{
	QPoint sceneStart = ui.edgePreview->pos();
	QPoint startPoint = QPoint(sceneStart.x() + 10, sceneStart.y() + 10);
	QPointF endPoint = _v2->pos();
	_config->NormalEdgeContext()->Size(val);
	_config->SelectedEdgeContext()->Size(val);
	int dx = -(endPoint.x() - startPoint.x());
	int dy = -(endPoint.y() - startPoint.y());
	float angle = std::atan2(dy, dx);
	
	delete _arrow;
	float size = _config->NormalEdgeContext()->Size();
	_arrow = new ArrowHeadImage(_edgePreview, .0f * size, 6.0f * size, angle * 180.0f / M_PI + 90, true);
	_arrow->setPos(endPoint);
	_arrow->setZValue(3);
	_arrow->setColor(_config->NormalEdgeContext()->Color());
	_arrow->setParentItem(_edgePreview);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorRedChanged(int val)
{
	QColor color = _config->NormalEdgeContext()->Color();
	color.setRed(val);
	_config->NormalEdgeContext()->Color(color);
	_arrow->setColor(_config->NormalEdgeContext()->Color());
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorGreenChanged(int val)
{
	QColor color = _config->NormalEdgeContext()->Color();
	color.setGreen(val);
	_config->NormalEdgeContext()->Color(color);
	_arrow->setColor(_config->NormalEdgeContext()->Color());
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorBlueChanged(int val)
{
	QColor color = _config->NormalEdgeContext()->Color();
	color.setBlue(val);
	_config->NormalEdgeContext()->Color(color);
	_arrow->setColor(_config->NormalEdgeContext()->Color());
	updateEdgePreview();
}

void GraphShapeDialog::acceptChanges()
{
	saveChanges();
	close();
}

void GraphShapeDialog::discardChanges()
{
	_config = _backupConfig->clone();
	close();
}

void GraphShapeDialog::applyChanges()
{
	saveChanges();
	ui.applyButton->setEnabled(false);
}

void GraphShapeDialog::saveChanges()
{
	_backupConfig = _config->clone();
}
