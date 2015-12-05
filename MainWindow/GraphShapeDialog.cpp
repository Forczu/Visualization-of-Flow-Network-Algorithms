#include "GraphShapeDialog.h"
#include "Config.h"
#include "VertexImage.h"
#include "GraphImage.h"
#include "UndirectedGraphImage.h"
#include "GraphScene.h"
#include "UnweightedEdgeStrategy.h"

GraphShapeDialog::GraphShapeDialog(GraphImage * graph, GraphConfig * config, QWidget *parent)
: QDialog(parent), _graph(graph)
{
	ui.setupUi(this);
	ui.applyButton->setEnabled(false);
	// utworzenie kopii zapasowej ustawieñ
	_config = config->clone();
	_backupConfig = config->clone();
	initVertexTab();
	initEdgeTab();
	createConnections();
}

GraphShapeDialog::~GraphShapeDialog()
{
	ui.edgePreview->scene()->removeItem(graph);
	delete graph;
	clearPreview(ui.vertexPreview);
	clearPreview(ui.edgePreview);
	delete _backupConfig;
	delete _vertex;
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
	QPoint startPoint = QPoint(0, 0);
	QGraphicsScene * graphScene = GraphScene::getInstance();
	ui.vertexPreview->setScene(graphScene);

	auto vertexConext = _config->NormalVertexContext();
	_verticePreview = new VertexImage(vertexConext);
	_verticePreview->setPos(startPoint);
	_verticePreview->setFlag(QGraphicsItem::ItemIsMovable, false);
	_verticePreview->setVertex(_vertex = new Vertex(1));
	graphScene->addItem(_verticePreview);
	ui.vertexPreview->centerOn(_verticePreview);

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
	int sceneWidth = ui.edgePreview->width();
	int sceneHeight = ui.edgePreview->height();
	QPoint startPoint = ui.edgePreview->pos();
	QPoint endPoint = QPoint(startPoint.x() + sceneWidth, startPoint.y() + sceneHeight);

	ui.edgePreview->setScene(GraphScene::getInstance());
	graph = new UndirectedGraphImage(_config);
	graph->setWeightStrategy(UnweightedEdgeStrategy::getInstance());
	graph->addVertex(startPoint);
	graph->addVertex(endPoint);
	graph->addEdge(1, 2, 0, EdgeType::StraightLine);
	ui.edgePreview->scene()->addItem(graph);
	ui.edgePreview->centerOn(graph);

	auto edgeContext = _backupConfig->NormalEdgeContext();
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
	_config->NormalEdgeContext()->Size(val);
	_config->SelectedEdgeContext()->Size(val);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorRedChanged(int val)
{
	QColor color = _config->NormalEdgeContext()->Color();
	color.setRed(val);
	_config->NormalEdgeContext()->Color(color);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorGreenChanged(int val)
{
	QColor color = _config->NormalEdgeContext()->Color();
	color.setGreen(val);
	_config->NormalEdgeContext()->Color(color);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorBlueChanged(int val)
{
	QColor color = _config->NormalEdgeContext()->Color();
	color.setBlue(val);
	_config->NormalEdgeContext()->Color(color);
	updateEdgePreview();
}

void GraphShapeDialog::acceptChanges()
{
	saveChanges();
	accept();
}

void GraphShapeDialog::discardChanges()
{
	reject();
}

void GraphShapeDialog::applyChanges()
{
	saveChanges();
	ui.applyButton->setEnabled(false);
}

void GraphShapeDialog::saveChanges()
{
	_graph->setConfig(_config->clone());
	delete _backupConfig;
	_backupConfig = _config->clone();
}
