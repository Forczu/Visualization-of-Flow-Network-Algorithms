#include "GraphShapeDialog.h"

GraphShapeDialog::GraphShapeDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	_currentContext = new VertexContext(Application::Config::Instance().DefaultVertexContext());

	QPoint startPoint = ui.vertexPreview->pos();
	int sceneWidth = ui.vertexPreview->width();
	int sceneHeight = ui.vertexPreview->height();

	_graphScene = new QGraphicsScene( startPoint.x(), startPoint.y(), sceneWidth, sceneHeight );
	ui.vertexPreview->setScene(_graphScene);

	_verticePreview = new VertexImage(*_currentContext);
	_verticePreview->setPos(startPoint.x() + sceneWidth / 2.0f, startPoint.y() + sceneHeight / 2.0f);
	_verticePreview->setFlag(QGraphicsItem::ItemIsMovable, false);
	_verticePreview->setVertex(new Vertex(1));
	_graphScene->addItem(_verticePreview);

	ui.vertexSizeSlider->setValue(_currentContext->Size());
	ui.strokeSizeSlider->setValue(_currentContext->StrokeSize());
	ui.vertexColorRedBox->setValue(_currentContext->Color().red());
	ui.vertexColorGreenBox->setValue(_currentContext->Color().green());
	ui.vertexColorBlueBox->setValue(_currentContext->Color().blue());
	ui.strokeColorRedBox->setValue(_currentContext->StrokeColor().red());
	ui.strokeColorGreenBox->setValue(_currentContext->StrokeColor().green());
	ui.strokeColorBlueBox->setValue(_currentContext->StrokeColor().blue());

	createConnections();

	ui.zastosujButton->setEnabled(false);
}

GraphShapeDialog::~GraphShapeDialog()
{
	delete _currentContext;
}

void GraphShapeDialog::updatePreview()
{
	_verticePreview->Context(*_currentContext);
	_graphScene->update();
}

void GraphShapeDialog::createConnections()
{
	connect(ui.vertexSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(verticeSizeChanged(int)));
	connect(ui.strokeSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(strokeSizeChanged(int)));
	connect(ui.vertexColorRedBox, SIGNAL(valueChanged(int)), this, SLOT(verticeColorRedChanged(int)));
	connect(ui.vertexColorGreenBox, SIGNAL(valueChanged(int)), this, SLOT(verticeColorBlueChanged(int)));
	connect(ui.vertexColorBlueBox, SIGNAL(valueChanged(int)), this, SLOT(verticeColorGreenChanged(int)));
	connect(ui.strokeColorRedBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorRedChanged(int)));
	connect(ui.strokeColorGreenBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorBlueChanged(int)));
	connect(ui.strokeColorBlueBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorGreenChanged(int)));
	connect(ui.okButton, SIGNAL(clicked(bool)), this, SLOT(acceptChanges()));
	connect(ui.anulujButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));
	connect(ui.zastosujButton, SIGNAL(clicked(bool)), this, SLOT(applyChanges()));
}

void GraphShapeDialog::enableApplyButton()
{
	if (!ui.zastosujButton->isEnabled())
		ui.zastosujButton->setEnabled(true);
}

void GraphShapeDialog::verticeSizeChanged(int val)
{
	_currentContext->Size(val);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::strokeSizeChanged(int val)
{
	_currentContext->StrokeSize(val);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::verticeColorRedChanged(int val)
{
	QColor color = _currentContext->Color();
	color.setRed(val);
	_currentContext->Color(color);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::verticeColorGreenChanged(int val)
{
	QColor color = _currentContext->Color();
	color.setGreen(val);
	_currentContext->Color(color);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::verticeColorBlueChanged(int val)
{
	QColor color = _currentContext->Color();
	color.setBlue(val);
	_currentContext->Color(color);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::strokeColorRedChanged(int val)
{
	QColor color = _currentContext->StrokeColor();
	color.setRed(val);
	_currentContext->StrokeColor(color);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::strokeColorGreenChanged(int val)
{
	QColor color = _currentContext->StrokeColor();
	color.setGreen(val);
	_currentContext->StrokeColor(color);
	updatePreview();
}

void GraphShapeDialog::strokeColorBlueChanged(int val)
{
	QColor color = _currentContext->StrokeColor();
	color.setBlue(val);
	_currentContext->StrokeColor(color);
	updatePreview();
	enableApplyButton();
}

void GraphShapeDialog::acceptChanges()
{
	Application::Config::Instance().DefaultVertexContext(*_currentContext);
	close();
}

void GraphShapeDialog::discardChanges()
{
	close();
}

void GraphShapeDialog::applyChanges()
{
	Application::Config::Instance().DefaultVertexContext(*_currentContext);
	ui.zastosujButton->setEnabled(false);
}
