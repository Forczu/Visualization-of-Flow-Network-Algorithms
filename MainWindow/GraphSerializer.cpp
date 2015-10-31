#include "GraphSerializer.h"

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

GraphImage * GraphSerializer::load(std::string const & filePath)
{
	GraphImage * graph;
	parse(filePath);
	xml_node<> * root = _doc.first_node("Graph");
	xml_node<> * configNode = root->first_node("Config");
	GraphConfig * config = deserializeConfig(configNode);
	xml_node<> * modelNode = root->first_node("Model");

	std::string type = root->first_attribute("type")->value();
	if (type == "directed")
		graph = new DirectedGraphImage(config, GraphScene::getInstance());
	deserializeModel(modelNode, graph);
	return graph;
}

bool GraphSerializer::save(GraphImage const & graph, std::string const & fileName)
{
	// zadeklarowanie xmla
	xml_node<> * decl = _doc.allocate_node(node_declaration);
	createAttribute(decl, "version", "1.0");
	createAttribute(decl, "encoding", "UTF-8");
	_doc.append_node(decl);
	// korzeñ
	xml_node<> * root = createNode("Graph");
	createAttribute(root, "type", "directed");
	createAttribute(root, "weighted", graph.Weighted() ? "True" : "False");
	_doc.append_node(root);
	// dane konfiguracyjne ca³ego grafu
	serializeConfig(graph.getConfig(), root);
	// struktura grafu
	serializeGraph(graph, root);

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
	xml_node<> * contextNode = createNode("VertexContext");
	createAttribute(contextNode, "type", childName);
	createAttribute(contextNode, "size", parseInt(context->Size()));
	createAttribute(contextNode, "strokeSize", parseInt(context->StrokeSize()));
	serializeColor(context->Color(), contextNode, "color");
	serializeColor(context->StrokeColor(), contextNode, "strokeColor");
	serializeFont(context->Font(), contextNode, "font");
	parentNode->append_node(contextNode);
	return true;
}

bool GraphSerializer::serializeEdgeContext(EdgeContext * context, xml_node<> * parentNode, const char * childName)
{
	xml_node<> * contextNode = createNode("EdgeContext");
	createAttribute(contextNode, "type", childName);
	createAttribute(contextNode, "size", parseInt(context->Size()));
	serializeColor(context->Color(), contextNode, "color");
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
	return std::atoi(str);
}

float GraphSerializer::toFloat(char * str)
{
	return std::atof(str);
}

bool GraphSerializer::toBool(char * str)
{
	return strcmp(str, "True") == 0;
}

void GraphSerializer::serializeColor(QColor const & color, xml_node<> * parentNode, const char * childName)
{
	xml_node<> * colorNode = createNode("Color");
	createAttribute(colorNode, "type", childName);
	createAttribute(colorNode, "r", parseInt(color.red()));
	createAttribute(colorNode, "g", parseInt(color.green()));
	createAttribute(colorNode, "b", parseInt(color.blue()));
	parentNode->append_node(colorNode);
}

void GraphSerializer::serializeFont(QFont const & font, xml_node<> * parentNode, char const * childName)
{
	xml_node<> * fontNode = createNode("Font");
	createAttribute(fontNode, "bold", font.bold() ? "True" : "False");
	createAttribute(fontNode, "size", parseInt(font.pointSize()));
	createAttribute(fontNode, "family", parseStdString(font.family().toStdString()));
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

void GraphSerializer::serializeConfig(GraphConfig * graphConfig, xml_node<> * parent)
{
	xml_node<> * config = createNode("Config");
	serializeVertexContext(graphConfig->NormalVertexContext(), config, "normal");
	serializeVertexContext(graphConfig->SelectedVertexContext(), config, "selected");
	serializeEdgeContext(graphConfig->NormalEdgeContext(), config, "normal");
	serializeEdgeContext(graphConfig->SelectedEdgeContext(), config, "normal");
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
	xml_node<> * vertexNode = createNode("Vertex", parent);
	createAttribute(vertexNode, "id", parseInt(vertex->getVertex()->Id()));
	createAttribute(vertexNode, "x", parseFloat(vertex->pos().x()));
	createAttribute(vertexNode, "y", parseFloat(vertex->pos().y()));
	PointMap map = vertex->getPoints();
	for (PointMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		serializePoint(*it, vertexNode);
	}
}

void GraphSerializer::serializeEdge(EdgeImage const * edge, xml_node<> * parent)
{
	xml_node<> * edgeNode = createNode("Edge", parent);
	Edge * e = edge->getEdge();
	createAttribute(edgeNode, "id", parseInt(e->Id()));
	createAttribute(edgeNode, "vertexFrom", parseInt(e->VertexFrom()->Id()));
	createAttribute(edgeNode, "vertexTo", parseInt(e->VertexTo()->Id()));
	createAttribute(edgeNode, "weight", parseInt(e->getWeight()));
	auto offset = edge->getOffset();
	createAttribute(edgeNode, "offsetType", offset.first ? "True" : "False");
	createAttribute(edgeNode, "offsetValue", parseFloat(offset.second));
	QPointF pos = edge->getText()->pos();
	createAttribute(edgeNode, "text.x", parseFloat(pos.x()));
	createAttribute(edgeNode, "text.y", parseFloat(pos.y()));
}

void GraphSerializer::serializeGraph(GraphImage const & graph, xml_node<> * parent)
{
	xml_node<> * graphNode = createNode("Model", parent);
	serializeVertices(graph.getVertices(), graphNode);
	serializeEdges(graph.getEdges(), graphNode);
}

void GraphSerializer::serializePoint(PointPair const & point, xml_node<> * parent)
{
	xml_node<> * pointNode = createNode("Point", parent);
	createAttribute(pointNode, "id", parseInt(point.first));
	createAttribute(pointNode, "x", parseInt(point.second.x()));
	createAttribute(pointNode, "y", parseInt(point.second.y()));
}

GraphConfig * GraphSerializer::deserializeConfig(xml_node<> * configNode)
{
	VertexContext *normalVertexContext, *selectedVertexContext;
	EdgeContext *normalEdgeContext, *selectedEdgeContext;
	for (xml_node<>* node = configNode->first_node("VertexContext"); node; node = node->next_sibling("VertexContext"))
	{
		if (strcmp(readAttribute(node, "type"), "normal") == 0)
		{
			normalVertexContext = deserializeVertexContext(node);
		}
		else if (strcmp(readAttribute(node, "type"), "selected") == 0)
		{
			selectedVertexContext = deserializeVertexContext(node);
		}
	}
	for (xml_node<>* node = configNode->first_node("EdgeContext"); node; node = node->next_sibling("EdgeContext"))
	{
		if (strcmp(readAttribute(node, "type"), "normal") == 0)
		{
			normalEdgeContext = deserializeEdgeContext(node);
		}
		else if (strcmp(readAttribute(node, "type"), "selected") == 0)
		{
			selectedEdgeContext = deserializeEdgeContext(node);
		}
	}
	return new GraphConfig(
		normalVertexContext, normalEdgeContext,
		selectedVertexContext, selectedEdgeContext);
}

VertexContext * GraphSerializer::deserializeVertexContext(xml_node<> * vertexNode)
{
	int size = toInt(readAttribute(vertexNode, "size"));
	int strokeSize = toInt(readAttribute(vertexNode, "strokeSize"));
	QColor color, strokeColor;
	for (xml_node<>* node = vertexNode->first_node("Color"); node; node = node->next_sibling("Color"))
	{
		if (strcmp(readAttribute(node, "type"), "color") == 0)
		{
			color = deserializeColor(node);
		}
		else if (strcmp(readAttribute(node, "type"), "strokeColor") == 0)
		{
			strokeColor = deserializeColor(node);
		}
	}
	QFont font = deserializeFont(vertexNode->first_node("Font"));
	VertexBuilder builder(size, strokeSize);
	builder.color(color)->strokeColor(strokeColor)->font(font);
	return builder.build();
}

QColor GraphSerializer::deserializeColor(xml_node<>* colorNode)
{
	int r = toInt(readAttribute(colorNode, "r"));
	int g = toInt(readAttribute(colorNode, "g"));
	int b = toInt(readAttribute(colorNode, "b"));
	return QColor(r, g, b);
}

QFont GraphSerializer::deserializeFont(xml_node<char> * fontNode)
{
	bool bold = toBool(readAttribute(fontNode, "bold"));
	int size = toInt(readAttribute(fontNode, "size"));
	QString family = readAttribute(fontNode, "family");
	QFont font;
	font.setBold(bold);
	font.setPointSize(size);
	font.setFamily(family);
	return font;
}

EdgeContext * GraphSerializer::deserializeEdgeContext(xml_node<>* edgeNode)
{
	int size = toInt(readAttribute(edgeNode, "size"));
	QColor color = deserializeColor(edgeNode->first_node("Color"));
	return new EdgeContext(size, color);
}

bool GraphSerializer::deserializeModel(xml_node<> * modelNode, GraphImage * graph)
{
	deserializeVertices(modelNode, graph);
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
	int id = toInt(readAttribute(vertexNode, "id"));
	float x = toFloat(readAttribute(vertexNode, "x"));
	float y = toFloat(readAttribute(vertexNode, "y"));
	PointMap points;
	for (xml_node<>* node = vertexNode->first_node("Point"); node; node = node->next_sibling("Point"))
	{
		deserializePoint(node, points);
	}
	graph->addVertex(id, QPointF(x, y), points);
}

void GraphSerializer::deserializePoint(xml_node<>* pointNode, PointMap & points)
{
	int id = toInt(readAttribute(pointNode, "id"));
	float x = toFloat(readAttribute(pointNode, "x"));
	float y = toFloat(readAttribute(pointNode, "y"));
	points[id] = QPointF(x, y);
}
