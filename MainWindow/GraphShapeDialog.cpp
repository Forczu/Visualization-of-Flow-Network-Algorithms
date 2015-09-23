#include "GraphShapeDialog.h"

GraphShapeDialog::GraphShapeDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	_currentContext = new VerticeContext(Application::Config::Instance().DefaultVerticeContext());

	QPoint startPoint = ui.verticePreview->pos();
	int sceneWidth = ui.verticePreview->width();
	int sceneHeight = ui.verticePreview->height();

	_graphScene = new QGraphicsScene( startPoint.x(), startPoint.y(), sceneWidth, sceneHeight );
	ui.verticePreview->setScene(_graphScene);

	_verticePreview = new VerticeImage(*_currentContext);
	_verticePreview->setPos(startPoint.x() + sceneWidth / 2.0f, startPoint.y() + sceneHeight / 2.0f);
	_verticePreview->setFlag(QGraphicsItem::ItemIsMovable, false);
	_verticePreview->setVertice(new Vertice(1));
	_graphScene->addItem(_verticePreview);

	ui.verticeSizeSlider->setValue(_currentContext->Size());
	ui.strokeSizeSlider->setValue(_currentContext->StrokeSize());
	ui.verticeColorRBox->setValue(_currentContext->Color().red());
	ui.verticeColorGBox->setValue(_currentContext->Color().green());
	ui.verticeColorBBox->setValue(_currentContext->Color().blue());
	ui.strokeColorRBox->setValue(_currentContext->StrokeColor().red());
	ui.strokeColorGBox->setValue(_currentContext->StrokeColor().green());
	ui.strokeColorBBox->setValue(_currentContext->StrokeColor().blue());

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
	connect(ui.verticeSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(verticeSizeChanged(int)));
	connect(ui.strokeSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(strokeSizeChanged(int)));
	connect(ui.verticeColorRBox, SIGNAL(valueChanged(int)), this, SLOT(verticeColorRedChanged(int)));
	connect(ui.verticeColorGBox, SIGNAL(valueChanged(int)), this, SLOT(verticeColorBlueChanged(int)));
	connect(ui.verticeColorBBox, SIGNAL(valueChanged(int)), this, SLOT(verticeColorGreenChanged(int)));
	connect(ui.strokeColorRBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorRedChanged(int)));
	connect(ui.strokeColorGBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorBlueChanged(int)));
	connect(ui.strokeColorBBox, SIGNAL(valueChanged(int)), this, SLOT(strokeColorGreenChanged(int)));
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
	Application::Config::Instance().DefaultVerticeContext(*_currentContext);
	close();
}

void GraphShapeDialog::discardChanges()
{
	close();
}

void GraphShapeDialog::applyChanges()
{
	Application::Config::Instance().DefaultVerticeContext(*_currentContext);
	ui.zastosujButton->setEnabled(false);
}
