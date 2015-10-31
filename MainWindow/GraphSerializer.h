#pragma once
#include <map>
#include <string>
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>
#include "DirectedGraphImage.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "Vertex.h"
#include "VertexImage.h"
#include "Graph.h"
#include "GraphScene.h"

using namespace rapidxml;

class GraphSerializer
{
protected:
	xml_document <> _doc;
	char * _contents;
	std::vector<char*> _atrVector;

public:
	GraphSerializer();
	~GraphSerializer();

	bool parse(std::string const & filePath);
	GraphImage * load(std::string const & filePath);
	bool save(GraphImage const & graph, std::string const & fileNam);

protected:
	char * stringToChar(std::string const & s);
	char * xmlToChar(std::string const & stageFile);

private:
#pragma region Serializacja
	bool serializeVertexContext(VertexContext * context, xml_node<> * parentNode, const char * childName);
	bool serializeEdgeContext(EdgeContext * context, xml_node<> * parentNode, const char * childName);
	void serializeColor(QColor const & color, xml_node<> * parentNode, const char * childName);
	void serializeFont(QFont const & font, xml_node<> * parentNode, char const * childName);
	void serializeConfig(GraphConfig * graphConfig, xml_node<> * parent);
	void serializeVertices(VertexImageMap const & map, xml_node<> * parent);
	void serializeEdges(EdgeImageMap const & map, xml_node<> * parent);
	void serializeVertex(VertexImage const * vertex, xml_node<> * parent);
	void serializeEdge(EdgeImage const * edge, xml_node<> * parent);
	void serializeGraph(GraphImage const & graph, xml_node<> * parent);
	void serializePoint(PointPair const & point, xml_node<> * parent);
#pragma endregion

#pragma region Deserializacja
	GraphConfig * deserializeConfig(xml_node<> * configNode);
	VertexContext * deserializeVertexContext(xml_node<> * vertexNode);
	QColor deserializeColor(xml_node<>* colorNode);
	QFont deserializeFont(xml_node<char> * fontNode);
	EdgeContext * deserializeEdgeContext(xml_node<>* edgeNode);
	bool deserializeModel(xml_node<> * modelNode, GraphImage * graph);
	void deserializeVertices(xml_node<> * modelNode, GraphImage * graph);
	void deserializeVertex(xml_node<>* node, GraphImage * graph);
	void deserializePoint(xml_node<>* node, PointMap & points);
#pragma endregion
	
	char * parseInt(int number);
	char * parseFloat(float value);
	char * parseStdString(std::string const & str);
	char * saveString(char const * str);

	int toInt(char * str);
	float toFloat(char * str);
	bool toBool(char * str);
	
	xml_node<> * createNode(char const * name);
	xml_node<> * createNode(char const * name, xml_node<> * parent);
	void createAttribute(xml_node<> * node, char const * name, char const * value);
	inline char * readAttribute(xml_node<> * node, char const * name);
};

