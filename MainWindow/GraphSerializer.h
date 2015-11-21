#pragma once
#include <map>
#include <string>
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>
#include <boost\lexical_cast.hpp>
#include "DirectedGraphImage.h"
#include "Edge.h"
#include "EdgeImage.h"
#include "Vertex.h"
#include "VertexImage.h"
#include "Graph.h"
#include "GraphScene.h"
#include "EdgeTextItem.h"

namespace Serialization
{
	class GraphSerializer;
	namespace Consts
	{
		// wêz³y
		static const char * ROOT =					"Graph";
		static const char * CONFIG_NODE =			"Config";
		static const char * MODEL_NODE =			"Model";
		static const char * COLOR_NODE =			"Color";
		static const char * FONT_NODE =				"Font";
		static const char * VERTEX_NODE =			"Vertex";
		static const char * EDGE_NODE =				"Edge";
		static const char * VERTEX_CONTEXT_NODE =	"VertexContext";
		static const char * EDGE_CONTEXT_NODE =		"EdgeContext";
		static const char * POINT_NODE =			"Point";
		static const char * POS_NODE =				"Position";
		static const char * EDGE_TEXT_ITEM_NODE =	"EdgeTextItem";
		static const char * NAME_NODE =				"Name";
		// atrybuty
		static const char * TYPE_ATR =				"type";
		static const char * WEIGHTED_ATR =			"weighted";
		static const char * SIZE_ATR =				"size";
		static const char * STROKE_SIZE_ATR =		"strokeSize";
		static const char * RED_ATR =				"r";
		static const char * GREEN_ATR =				"g";
		static const char * BLUE_ATR =				"b";
		static const char * BOLD_ATR =				"bold";
		static const char * FAMILY_ATR =			"family";
		static const char * ID_ATR =				"id";
		static const char * POS_X_ATR =				"x";
		static const char * POS_Y_ATR =				"y";
		static const char * VERTEX_FROM_ATR =		"vertexFrom";
		static const char * VERTEX_TO_ATR =			"vertexTo";
		static const char * WEIGHT_ATR =			"weight";
		static const char * OFFSET_TYPE_ATR =		"offsetType";
		static const char * OFFSET_VAL_ATR =		"offsetValue";
		// wartoœci
		static const char * DIRECTED_VAL =			"directed";
		static const char * TRUE_VAL =				"true";
		static const char * FALSE_VAL =				"false";
		static const char * NORMAL_VAL =			"normal";
		static const char * SELECTED_VAL =			"selected";
		static const char * COLOR_VAL =				"color";
		static const char * STROKE_COLOR_VAL =		"strokeColor";
		static const char * STRAIGHT_VAL =			"straight";
		static const char * BEZIER_VAL =			"bezier";
	}
}

using namespace rapidxml;
using namespace Serialization::Consts;

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
	GraphImage * deserialize(std::string const & filePath);
	bool serialize(GraphImage const & graph, std::string const & fileNam);

private:
	char * stringToChar(std::string const & s);
	char * xmlToChar(std::string const & stageFile);
	xml_node<> * createNode(char const * name);
	xml_node<> * createNode(char const * name, xml_node<> * parent);
	void createAttribute(xml_node<> * node, char const * name, char const * value);
	inline char * readAttribute(xml_node<> * node, char const * name);
	void createValue(xml_node<>* node, char const * value);
	char * readValue(xml_node<> * node);
#pragma region Serializacja
	bool serializeVertexContext(VertexContext * context, xml_node<> * parentNode, const char * childName);
	bool serializeEdgeContext(EdgeContext * context, xml_node<> * parentNode, const char * childName);
	void serializeColor(QColor const & color, xml_node<> * parentNode, const char * childName);
	void serializeFont(QFont const & font, xml_node<> * parentNode, char const * childName);
	void serializeConfig(GraphConfig * graphConfig, xml_node<> * parent);
	void serializeVertices(VertexImageMap const & map, xml_node<> * parent);
	void serializeEdges(EdgeImageMap const & map, xml_node<> * parent);
	void serializeVertex(VertexImage const * vertex, xml_node<> * parent);
	void serializeEdge(EdgeImage * edge, xml_node<> * parent);
	void serializeModel(GraphImage const & graph, xml_node<> * parent);
	void serializePoint(PointPair const & point, xml_node<> * parent);
	void serializePosition(QPointF const & position, xml_node<> * parent);
	void serializeTextItem(EdgeTextItem const * textItem, char const * value, xml_node<> * parent);
#pragma endregion

#pragma region Deserializacja
	GraphConfig * deserializeConfig(xml_node<> * configNode);
	VertexContext * deserializeVertexContext(xml_node<> * vertexNode);
	QColor deserializeColor(xml_node<>* colorNode);
	QFont deserializeFont(xml_node<char> * fontNode);
	EdgeContext * deserializeEdgeContext(xml_node<>* edgeNode);
	QPointF deserializePosition(xml_node<>* posNode);
	bool deserializeModel(xml_node<> * modelNode, GraphImage * graph);
	void deserializeVertices(xml_node<> * modelNode, GraphImage * graph);
	void deserializeVertex(xml_node<>* node, GraphImage * graph);
	void deserializePoint(xml_node<>* node, PointMap & points);
	void deserializeEdges(xml_node<> * modelNode, GraphImage * graph);
	void deserializeEdge(xml_node<>* node, GraphImage * graph);
	void deserializeTextItem(xml_node<>* node, EdgeImage * edge);
#pragma endregion
	
#pragma region Parsowanie
	char * parseInt(int number);
	char * parseFloat(float value);
	char * parseStdString(std::string const & str);
	char * saveString(char const * str);
	int toInt(char * str);
	float toFloat(char * str);
	bool toBool(char * str);
#pragma endregion
};

