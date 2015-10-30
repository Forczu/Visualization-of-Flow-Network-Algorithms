#include "GraphSerializer.h"
#include "DirectedGraphImage.h"

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
		_doc.parse<0>(const_cast<char*>(_contents));
		return true;
	}
	catch (rapidxml::parse_error e)
	{
		return false;
	}
}

bool GraphSerializer::save(DirectedGraphImage const & graph)
{
	// zadeklarowanie xmla
	xml_node<> * decl = _doc.allocate_node(node_declaration);
	createAttribute(decl, "version", "1.0");
	createAttribute(decl, "encoding", "UTF-8");
	_doc.append_node(decl);
	// korzeñ
	xml_node<> * root = createNode("DirectedGraph");
	createAttribute(root, "weighted", graph.Weighted() ? "True" : "False");
	_doc.append_node(root);
	// dane konfiguracyjne ca³ego grafu
	GraphConfig * graphConfig = graph.getConfig();
	xml_node<> * config = createNode("Config");
	serializeVertexContext(graphConfig->NormalVertexContext(), config, "normal");
	serializeVertexContext(graphConfig->SelectedVertexContext(), config, "selected");
	serializeEdgeContext(graphConfig->NormalEdgeContext(), config, "normal");
	serializeEdgeContext(graphConfig->SelectedEdgeContext(), config, "normal");
	root->append_node(config);

	std::ofstream file_stored("serialized_graph.xml");
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

void GraphSerializer::createAttribute(xml_node<> * node, char const * name, char const * value)
{
	node->append_attribute(_doc.allocate_attribute(name, value));
}