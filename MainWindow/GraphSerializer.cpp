#include "GraphSerializer.h"
#include "StraightEdgeImage.h"
#include "BezierEdgeImage.h"
#include <typeinfo>

GraphSerializer::GraphSerializer() : _contents(nullptr)
{

}

GraphSerializer::~GraphSerializer()
{
	for (std::vector<char*>::iterator it = _atrVector.begin(); it != _atrVector.end(); ++it)
	{
		free(*it);
	}
}

bool GraphSerializer::parse(std::string const & filePath)
{
	_contents = xmlToChar(filePath);
	if (_contents == nullptr)
		return false;
	try
	{
		_doc.parse<0>(_contents);
		return true;
	}
	catch (rapidxml::parse_error e)
	{
		return false;
	}
}

GraphImage * GraphSerializer::deserialize(std::string const & filePath)
{
	GraphImage * graph;
	parse(filePath);
	xml_node<> * root = _doc.first_node(ROOT);
	xml_node<> * configNode = root->first_node(CONFIG_NODE);
	GraphConfig * config = deserializeConfig(configNode);
	xml_node<> * modelNode = root->first_node(MODEL_NODE);
	std::string type = root->first_attribute(TYPE_ATR)->value();
	if (type == DIRECTED_VAL)
		graph = new DirectedGraphImage(config);
	deserializeModel(modelNode, graph);
	return graph;
}

bool GraphSerializer::serialize(GraphImage const & graph, std::string const & fileName)
{
	// zadeklarowanie xmla
	xml_node<> * decl = _doc.allocate_node(node_declaration);
	createAttribute(decl, "version", "1.0");
	createAttribute(decl, "encoding", "UTF-8");
	_doc.append_node(decl);
	// korzeñ
	xml_node<> * root = createNode(ROOT);
	createAttribute(root, TYPE_ATR, DIRECTED_VAL);
	createAttribute(root, WEIGHTED_ATR, graph.setWeighted() ? TRUE_VAL : FALSE_VAL);
	_doc.append_node(root);
	// dane konfiguracyjne ca³ego grafu
	serializeConfig(graph.getConfig(), root);
	// struktura grafu
	serializeModel(graph, root);

	std::ofstream file_stored(fileName);
	file_stored << _doc;
	file_stored.close();
	return true;
}

char * GraphSerializer::stringToChar(std::string const & s)
{
	unsigned int N = s.length();
	char * out = new char[N + 1];
	std::copy(s.c_str(), (s.c_str() + N - 1), out);
	return out;
}

char * GraphSerializer::xmlToChar(std::string const & stageFile)
{
	std::ifstream file(stageFile);
	if (file.fail())
		return nullptr;
	std::filebuf * pbuf = file.rdbuf();
	long fileLength = static_cast<long>(pbuf->pubseekoff(0, std::ios::end, std::ios::in));
	file.seekg(0);
	char * out = new char[fileLength + 1];
	file.read(out, fileLength);
	return out;
}

bool GraphSerializer::serializeVertexContext(VertexContext * context, xml_node<> * parentNode, const char * childName)
{
	xml_node<> * contextNode = createNode(VERTEX_CONTEXT_NODE);
	createAttribute(contextNode, TYPE_ATR, childName);
	createAttribute(contextNode, SIZE_ATR, parseInt(context->Size()));
	createAttribute(contextNode, STROKE_SIZE_ATR, parseInt(context->StrokeSize()));
	serializeColor(context->Color(), contextNode, COLOR_VAL);
	serializeColor(context->StrokeColor(), contextNode, STROKE_COLOR_VAL);
	serializeFont(context->Font(), contextNode, "font");
	parentNode->append_node(contextNode);
	return true;
}

bool GraphSerializer::serializeEdgeContext(EdgeContext * context, xml_node<> * parentNode, const char * childName)
{
	xml_node<> * contextNode = createNode(EDGE_CONTEXT_NODE);
	createAttribute(contextNode, TYPE_ATR, childName);
	createAttribute(contextNode, SIZE_ATR, parseInt(context->Size()));
	serializeColor(context->Color(), contextNode, COLOR_VAL);
	parentNode->append_node(contextNode);
	return true;
}

char * GraphSerializer::parseInt(int number)
{
	std::string s = std::to_string(number);
	return parseStdString(s);
}

char * GraphSerializer::parseFloat(float value)
{
	std::string s = std::to_string(value);
	return parseStdString(s);
}

char * GraphSerializer::parseStdString(std::string const & s)
{
	char const * str = s.c_str();
	return saveString(str);
}

char * GraphSerializer::saveString(char const * str)
{
	char * c = (char*)malloc(strlen(str) + 1);
	strcpy(c, str);
	_atrVector.push_back(c);
	return c;
}

int GraphSerializer::toInt(char * str)
{
	return boost::lexical_cast<int>(str);
}

float GraphSerializer::toFloat(char * str)
{
	return boost::lexical_cast<float>(str);
}

bool GraphSerializer::toBool(char * str)
{
	return strcmp(str, TRUE_VAL) == 0;
}

void GraphSerializer::serializeColor(QColor const & color, xml_node<> * parentNode, const char * childName)
{
	xml_node<> * colorNode = createNode(COLOR_NODE);
	createAttribute(colorNode, TYPE_ATR, childName);
	createAttribute(colorNode, RED_ATR, parseInt(color.red()));
	createAttribute(colorNode, GREEN_ATR, parseInt(color.green()));
	createAttribute(colorNode, BLUE_ATR, parseInt(color.blue()));
	parentNode->append_node(colorNode);
}

void GraphSerializer::serializeFont(QFont const & font, xml_node<> * parentNode, char const * childName)
{
	xml_node<> * fontNode = createNode(FONT_NODE);
	createAttribute(fontNode, BOLD_ATR, font.bold() ? TRUE_VAL : FALSE_VAL);
	createAttribute(fontNode, SIZE_ATR, parseInt(font.pointSize()));
	createAttribute(fontNode, FAMILY_ATR, parseStdString(font.family().toStdString()));
	parentNode->append_node(fontNode);
}

xml_node<> * GraphSerializer::createNode(char const * name)
{
	return _doc.allocate_node(node_element, name);
}

xml_node<> * GraphSerializer::createNode(char const * name, xml_node<> * parent)
{
	xml_node<> * node = createNode(name);
	parent->append_node(node);
	return node;
}

void GraphSerializer::createAttribute(xml_node<> * node, char const * name, char const * value)
{
	node->append_attribute(_doc.allocate_attribute(name, value));
}

char * GraphSerializer::readAttribute(xml_node<> * node, char const * name)
{
	return node->first_attribute(name)->value();
}

void GraphSerializer::createValue(xml_node<>* node, char const * value)
{
	node->value(_doc.allocate_string(value));
}

char * GraphSerializer::readValue(xml_node<> * node)
{
	return node->value();
}

void GraphSerializer::serializeConfig(GraphConfig * graphConfig, xml_node<> * parent)
{
	xml_node<> * config = createNode(CONFIG_NODE);
	xml_node<>* nameNode = createNode(NAME_NODE, config);
	createValue(nameNode, graphConfig->getName().toStdString().c_str());
	serializeVertexContext(graphConfig->NormalVertexContext(), config, NORMAL_VAL);
	serializeVertexContext(graphConfig->SelectedVertexContext(), config, SELECTED_VAL);
	serializeEdgeContext(graphConfig->NormalEdgeContext(), config, NORMAL_VAL);
	serializeEdgeContext(graphConfig->SelectedEdgeContext(), config, SELECTED_VAL);
	parent->append_node(config);
}

void GraphSerializer::serializeVertices(VertexImageMap const & map, xml_node<> * parent)
{
	for (VertexImageMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		serializeVertex((*it).second, parent);
	}
}

void GraphSerializer::serializeEdges(EdgeImageMap const & map, xml_node<> * parent)
{
	for (EdgeImageMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		serializeEdge((*it).second, parent);
	}
}

void GraphSerializer::serializeVertex(VertexImage const * vertex, xml_node<> * parent)
{
	xml_node<> * vertexNode = createNode(VERTEX_NODE, parent);
	createAttribute(vertexNode, ID_ATR, parseInt(vertex->getVertex()->Id()));
	serializePosition(vertex->pos(), vertexNode);
	PointMap map = vertex->getPoints();
	for (PointMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		serializePoint(*it, vertexNode);
	}
}

void GraphSerializer::serializePosition(QPointF const & position, xml_node<> * parent)
{
	xml_node<> * posNode = createNode(POS_NODE, parent);
	createAttribute(posNode, POS_X_ATR, parseFloat(position.x()));
	createAttribute(posNode, POS_Y_ATR, parseFloat(position.y()));
}

void GraphSerializer::serializeTextItem(EdgeTextItem const * textItem, char const * value, xml_node<> * parent)
{
	xml_node<> * posNode = createNode(EDGE_TEXT_ITEM_NODE, parent);
	serializePosition(textItem->pos(), posNode);
}

void GraphSerializer::serializeEdge(EdgeImage * edge, xml_node<> * parent)
{
	xml_node<> * edgeNode = createNode(EDGE_NODE, parent);
	char const * type;
	if (dynamic_cast<StraightEdgeImage*>(edge) != NULL)
		type = STRAIGHT_VAL;
	else if (dynamic_cast<BezierEdgeImage*>(edge) != NULL)
		type = BEZIER_VAL;
	createAttribute(edgeNode, TYPE_ATR, type);
	Edge * e = edge->getEdge();
	createAttribute(edgeNode, ID_ATR, parseInt(e->Id()));
	createAttribute(edgeNode, VERTEX_FROM_ATR, parseInt(e->VertexFrom()->Id()));
	createAttribute(edgeNode, VERTEX_TO_ATR, parseInt(e->VertexTo()->Id()));
	const char * capacity = parseInt(e->getCapacity());
	createAttribute(edgeNode, WEIGHT_ATR, capacity);
	auto offset = edge->getOffset();
	createAttribute(edgeNode, OFFSET_TYPE_ATR, offset.first ? TRUE_VAL : FALSE_VAL);
	createAttribute(edgeNode, OFFSET_VAL_ATR, parseFloat(offset.second));
	serializeTextItem(edge->getTextItem(), capacity, edgeNode);
}

void GraphSerializer::serializeModel(GraphImage const & graph, xml_node<> * parent)
{
	xml_node<> * graphNode = createNode(MODEL_NODE, parent);
	serializeVertices(graph.getVertices(), graphNode);
	serializeEdges(graph.getEdges(), graphNode);
}

void GraphSerializer::serializePoint(PointPair const & point, xml_node<> * parent)
{
	xml_node<> * pointNode = createNode(POINT_NODE, parent);
	createAttribute(pointNode, ID_ATR, parseInt(point.first));
	createAttribute(pointNode, POS_X_ATR, parseInt(point.second.x()));
	createAttribute(pointNode, POS_Y_ATR, parseInt(point.second.y()));
}

GraphConfig * GraphSerializer::deserializeConfig(xml_node<> * configNode)
{
	VertexContext *normalVertexContext, *selectedVertexContext;
	EdgeContext *normalEdgeContext, *selectedEdgeContext;
	QString name = readValue(configNode->first_node(NAME_NODE));
	for (xml_node<>* node = configNode->first_node(VERTEX_CONTEXT_NODE); node; node = node->next_sibling(VERTEX_CONTEXT_NODE))
	{
		if (strcmp(readAttribute(node, TYPE_ATR), NORMAL_VAL) == 0)
		{
			normalVertexContext = deserializeVertexContext(node);
		}
		else if (strcmp(readAttribute(node, TYPE_ATR), SELECTED_VAL) == 0)
		{
			selectedVertexContext = deserializeVertexContext(node);
		}
	}
	for (xml_node<>* node = configNode->first_node(EDGE_CONTEXT_NODE); node; node = node->next_sibling(EDGE_CONTEXT_NODE))
	{
		if (strcmp(readAttribute(node, TYPE_ATR), NORMAL_VAL) == 0)
		{
			normalEdgeContext = deserializeEdgeContext(node);
		}
		else if (strcmp(readAttribute(node, TYPE_ATR), SELECTED_VAL) == 0)
		{
			selectedEdgeContext = deserializeEdgeContext(node);
		}
	}
	GraphConfig * config = new GraphConfig(
		normalVertexContext, normalEdgeContext,
		selectedVertexContext, selectedEdgeContext);
	config->setName(name);
	return config;
}

VertexContext * GraphSerializer::deserializeVertexContext(xml_node<> * vertexNode)
{
	int size = toInt(readAttribute(vertexNode, SIZE_ATR));
	int strokeSize = toInt(readAttribute(vertexNode, STROKE_SIZE_ATR));
	QColor color, strokeColor;
	for (xml_node<>* node = vertexNode->first_node(COLOR_NODE); node; node = node->next_sibling(COLOR_NODE))
	{
		if (strcmp(readAttribute(node, TYPE_ATR), COLOR_VAL) == 0)
		{
			color = deserializeColor(node);
		}
		else if (strcmp(readAttribute(node, TYPE_ATR), STROKE_COLOR_VAL) == 0)
		{
			strokeColor = deserializeColor(node);
		}
	}
	QFont font = deserializeFont(vertexNode->first_node(FONT_NODE));
	VertexBuilder builder(size, strokeSize);
	builder.color(color)->strokeColor(strokeColor)->font(font);
	return builder.build();
}

QColor GraphSerializer::deserializeColor(xml_node<>* colorNode)
{
	int r = toInt(readAttribute(colorNode, RED_ATR));
	int g = toInt(readAttribute(colorNode, GREEN_ATR));
	int b = toInt(readAttribute(colorNode, BLUE_ATR));
	return QColor(r, g, b);
}

QFont GraphSerializer::deserializeFont(xml_node<char> * fontNode)
{
	bool bold = toBool(readAttribute(fontNode, BOLD_ATR));
	int size = toInt(readAttribute(fontNode, SIZE_ATR));
	QString family = readAttribute(fontNode, FAMILY_ATR);
	QFont font;
	font.setBold(bold);
	font.setPointSize(size);
	font.setFamily(family);
	return font;
}

EdgeContext * GraphSerializer::deserializeEdgeContext(xml_node<>* edgeNode)
{
	int size = toInt(readAttribute(edgeNode, SIZE_ATR));
	QColor color = deserializeColor(edgeNode->first_node(COLOR_NODE));
	return new EdgeContext(size, color);
}

QPointF GraphSerializer::deserializePosition(xml_node<>* posNode)
{
	float x = toFloat(readAttribute(posNode, POS_X_ATR));
	float y = toFloat(readAttribute(posNode, POS_Y_ATR));
	return QPointF(x, y);
}

bool GraphSerializer::deserializeModel(xml_node<> * modelNode, GraphImage * graph)
{
	deserializeVertices(modelNode, graph);
	deserializeEdges(modelNode, graph);
	return true;
}

void GraphSerializer::deserializeVertices(xml_node<> * modelNode, GraphImage * graph)
{
	for (xml_node<>* node = modelNode->first_node("Vertex"); node; node = node->next_sibling("Vertex"))
	{
		deserializeVertex(node, graph);
	}
}

void GraphSerializer::deserializeVertex(xml_node<>* vertexNode, GraphImage * graph)
{
	int id = toInt(readAttribute(vertexNode, ID_ATR));
	QPointF position = deserializePosition(vertexNode->first_node(POS_NODE));
	PointMap points;
	for (xml_node<>* node = vertexNode->first_node(POINT_NODE); node; node = node->next_sibling(POINT_NODE))
	{
		deserializePoint(node, points);
	}
	graph->addVertex(id, position, points);
}

void GraphSerializer::deserializePoint(xml_node<>* pointNode, PointMap & points)
{
	int id = toInt(readAttribute(pointNode, ID_ATR));
	float x = toFloat(readAttribute(pointNode, POS_X_ATR));
	float y = toFloat(readAttribute(pointNode, POS_Y_ATR));
	points[id] = QPointF(x, y);
}

void GraphSerializer::deserializeEdges(xml_node<> * modelNode, GraphImage * graph)
{
	for (xml_node<>* node = modelNode->first_node(EDGE_NODE); node; node = node->next_sibling(EDGE_NODE))
	{
		deserializeEdge(node, graph);
	}
}

void GraphSerializer::deserializeEdge(xml_node<>* node, GraphImage * graph)
{
	EdgeImage * edge;
	EdgeType edgeType;
	std::string type = readAttribute(node, TYPE_ATR);
	if (type == STRAIGHT_VAL)
		edgeType = EdgeType::StraightLine;
	else if (type == BEZIER_VAL)
		edgeType = EdgeType::BezierLine;
	int vertexFrom = toInt(readAttribute(node, VERTEX_FROM_ATR));
	int vertexTo = toInt(readAttribute(node, VERTEX_TO_ATR));
	int weight = toInt(readAttribute(node, WEIGHT_ATR));
	bool offsetType = toBool(readAttribute(node, OFFSET_TYPE_ATR));
	float offsetValue = toFloat(readAttribute(node, OFFSET_VAL_ATR));
	edge = graph->addEdge(vertexFrom, vertexTo, weight, edgeType);
	edge->setOffset(offsetType, offsetValue);
	deserializeTextItem(node->first_node(EDGE_TEXT_ITEM_NODE), edge);
}

void GraphSerializer::deserializeTextItem(xml_node<>* node, EdgeImage * edge)
{
	QPointF position = deserializePosition(node->first_node(POS_NODE));
	edge->getTextItem()->setPos(position);
}
