#include "GraphShapeDialog.h"

GraphShapeDialog::GraphShapeDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	ui.applyButton->setEnabled(false);
	_defaultVertexContext = new VertexContext(Application::Config::Instance().DefaultVertexContext());
	_defaultEdgeContext = new EdgeContext(Application::Config::Instance().DefaultEdgeContext());
	_selectedVertexContext = new VertexContext(Application::Config::Instance().SelectedVertexContext());
	_selectedEdgeContext = new EdgeContext(Application::Config::Instance().SelectedEdgeContext());
	initVertexTab();
	initEdgeTab();
	createConnections();
}

GraphShapeDialog::~GraphShapeDialog()
{
	clearPreview(ui.vertexPreview);
	clearPreview(ui.edgePreview);
	delete _defaultVertexContext;
	delete _defaultEdgeContext;
	delete _selectedVertexContext;
	delete _selectedEdgeContext;
	delete _v1;
	delete _v2;
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

	_verticePreview = new VertexImage(*_defaultVertexContext);
	_verticePreview->setPos(startPoint.x() + sceneWidth / 2.0f, startPoint.y() + sceneHeight / 2.0f);
	_verticePreview->setFlag(QGraphicsItem::ItemIsMovable, false);
	_verticePreview->setVertex(new Vertex(1));
	graphScene->addItem(_verticePreview);

	ui.vertexSizeSlider->setValue(_defaultVertexContext->Size());
	ui.strokeSizeSlider->setValue(_defaultVertexContext->StrokeSize());
	ui.vertexColorRedBox->setValue(_defaultVertexContext->Color().red());
	ui.vertexColorGreenBox->setValue(_defaultVertexContext->Color().green());
	ui.vertexColorBlueBox->setValue(_defaultVertexContext->Color().blue());
	ui.strokeColorRedBox->setValue(_defaultVertexContext->StrokeColor().red());
	ui.strokeColorGreenBox->setValue(_defaultVertexContext->StrokeColor().green());
	ui.strokeColorBlueBox->setValue(_defaultVertexContext->StrokeColor().blue());
}

void GraphShapeDialog::initEdgeTab()
{
	QPoint sceneStart = ui.edgePreview->pos();
	int sceneWidth = ui.edgePreview->width();
	int sceneHeight = ui.edgePreview->height();
	QPoint startPoint = QPoint(sceneStart.x() + 30, sceneStart.y() + 30);
	QPoint endPoint = QPoint(startPoint.x() + sceneWidth - 60, startPoint.y() + sceneHeight - 60);

	QGraphicsScene * graphScene = new QGraphicsScene(sceneStart.x(), sceneStart.y(), sceneWidth, sceneHeight);
	ui.edgePreview->setScene(graphScene);

	_v1 = new VertexImage(*_defaultVertexContext);
	_v2 = new VertexImage(*_defaultVertexContext);
	_v1->setPos(startPoint);
	_v2->setPos(endPoint);
	_edgePreview = new StraightEdgeImage(_v1, _v2, *_defaultEdgeContext);
	graphScene->addItem(_edgePreview);

	ui.edgeSizeSlider->setValue(_defaultEdgeContext->Size());
	ui.edgeColorRedBox->setValue(_defaultEdgeContext->Color().red());
	ui.edgeColorGreenBox->setValue(_defaultEdgeContext->Color().green());
	ui.edgeColorBlueBox->setValue(_defaultEdgeContext->Color().blue());
}

void GraphShapeDialog::updateVertexPreview()
{
	_verticePreview->Context(*_defaultVertexContext);
	ui.vertexPreview->viewport()->update();
	enableApplyButton();
}

void GraphShapeDialog::updateEdgePreview()
{
	_edgePreview->Context(*_defaultEdgeContext);
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
	_defaultVertexContext->Size(val);
	_selectedVertexContext->Size(val);
	updateVertexPreview();
}

void GraphShapeDialog::strokeSizeChanged(int val)
{
	_defaultVertexContext->StrokeSize(val);
	_selectedVertexContext->StrokeSize(val);
	updateVertexPreview();
}

void GraphShapeDialog::vertexColorRedChanged(int val)
{
	QColor color = _defaultVertexContext->Color();
	color.setRed(val);
	_defaultVertexContext->Color(color);
	updateVertexPreview();
}

void GraphShapeDialog::vertexColorGreenChanged(int val)
{
	QColor color = _defaultVertexContext->Color();
	color.setGreen(val);
	_defaultVertexContext->Color(color);
	updateVertexPreview();
}

void GraphShapeDialog::vertexColorBlueChanged(int val)
{
	QColor color = _defaultVertexContext->Color();
	color.setBlue(val);
	_defaultVertexContext->Color(color);
	updateVertexPreview();
}

void GraphShapeDialog::strokeColorRedChanged(int val)
{
	QColor color = _defaultVertexContext->StrokeColor();
	color.setRed(val);
	_defaultVertexContext->StrokeColor(color);
	updateVertexPreview();
}

void GraphShapeDialog::strokeColorGreenChanged(int val)
{
	QColor color = _defaultVertexContext->StrokeColor();
	color.setGreen(val);
	_defaultVertexContext->StrokeColor(color);
	updateVertexPreview();
}

void GraphShapeDialog::strokeColorBlueChanged(int val)
{
	QColor color = _defaultVertexContext->StrokeColor();
	color.setBlue(val);
	_defaultVertexContext->StrokeColor(color);
	updateVertexPreview();
}

void GraphShapeDialog::edgeSizeChanged(int val)
{
	_defaultEdgeContext->Size(val);
	_selectedEdgeContext->Size(val);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorRedChanged(int val)
{
	QColor color = _defaultEdgeContext->Color();
	color.setRed(val);
	_defaultEdgeContext->Color(color);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorGreenChanged(int val)
{
	QColor color = _defaultEdgeContext->Color();
	color.setGreen(val);
	_defaultEdgeContext->Color(color);
	updateEdgePreview();
}

void GraphShapeDialog::edgeColorBlueChanged(int val)
{
	QColor color = _defaultEdgeContext->Color();
	color.setBlue(val);
	_defaultEdgeContext->Color(color);
	updateEdgePreview();
}

void GraphShapeDialog::acceptChanges()
{
	saveChanges();
	close();
}

void GraphShapeDialog::discardChanges()
{
	close();
}

void GraphShapeDialog::applyChanges()
{
	saveChanges();
	ui.applyButton->setEnabled(false);
}

void GraphShapeDialog::saveChanges()
{
	Application::Config::Instance().DefaultVertexContext(*_defaultVertexContext);
	Application::Config::Instance().DefaultEdgeContext(*_defaultEdgeContext);
	Application::Config::Instance().SelectedVertexContext(*_selectedVertexContext);
	Application::Config::Instance().SelectedEdgeContext(*_selectedEdgeContext);
}
