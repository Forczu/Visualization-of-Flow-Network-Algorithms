#include "GraphSerializer.h"

GraphSerializer::GraphSerializer() : _contents(nullptr)
{

}

GraphSerializer::~GraphSerializer()
{
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
	decl->append_attribute(_doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(_doc.allocate_attribute("encoding", "utf-8"));
	_doc.append_node(decl);
	// korzeñ
	xml_node<> * root = _doc.allocate_node(node_element, "DirectedGraph");
	root->append_attribute(_doc.allocate_attribute("weighted", graph.Weighted() ? "True" : "False"));
	_doc.append_node(root);
	// dane konfiguracyjne ca³ego grafu
	GraphConfig * graphConfig = graph.getConfig();
	xml_node<> * config = _doc.allocate_node(node_element, "Config");
	serializeVertexContext(graphConfig->NormalVertexContext(), config, "normalVertexContext");
	root->append_node(config);

	std::string xml_as_string;
	print(std::back_inserter(xml_as_string), _doc);

	std::ofstream file_stored("serialized_graph.xml");
	file_stored << xml_as_string;
	file_stored.close();
	_doc.clear();
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

bool GraphSerializer::serializeVertexContext(VertexContext * context, xml_node<> * parentNode, std::string const & childName)
{
	xml_node<> * contextNode = _doc.allocate_node(node_element, childName.c_str());
	int size = context->Size();
	auto sizeStr = std::to_string(size);
	const char * constAtr = sizeStr.c_str();
	char * atr = const_cast<char*>(constAtr);
	contextNode->append_attribute(_doc.allocate_attribute("size", atr));
	contextNode->append_attribute(_doc.allocate_attribute("strokeSize", std::to_string(context->StrokeSize()).c_str()));
	serializeColor(context->Color(), contextNode, "color");
	serializeColor(context->StrokeColor(), contextNode, "strokeColor");
	serializeFont(context->Font(), contextNode, "font");
	contextNode->append_attribute(_doc.allocate_attribute("strokeSize", std::to_string(context->StrokeSize()).c_str()));
	contextNode->append_attribute(_doc.allocate_attribute("style", std::to_string(context->Style()).c_str()));
	parentNode->append_node(contextNode);
	return true;
}

void GraphSerializer::serializeColor(QColor const & color, xml_node<> * parentNode, const char * childName)
{
	xml_node<> * colorNode = _doc.allocate_node(node_element, childName);
	colorNode->append_attribute(_doc.allocate_attribute("r", std::to_string(color.red()).c_str()));
	colorNode->append_attribute(_doc.allocate_attribute("g", std::to_string(color.green()).c_str()));
	colorNode->append_attribute(_doc.allocate_attribute("b", std::to_string(color.blue()).c_str()));
	parentNode->append_node(colorNode);
}

void GraphSerializer::serializeFont(QFont const & font, xml_node<> * parentNode, char const * childName)
{
	xml_node<> * fontNode = _doc.allocate_node(node_element, childName);
	fontNode->append_attribute(_doc.allocate_attribute("bold", font.bold() ? "True" : "False"));
	fontNode->append_attribute(_doc.allocate_attribute("size", std::to_string(font.pointSize()).c_str()));
	fontNode->append_attribute(_doc.allocate_attribute("family", font.family().toStdString().c_str()));
	parentNode->append_node(fontNode);
}
