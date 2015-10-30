#pragma once
#include <string>
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>
#include "DirectedGraphImage.h"

using namespace rapidxml;

class GraphSerializer
{
protected:
	xml_document <> _doc;
	char const * _contents;

public:
	GraphSerializer();
	~GraphSerializer();

	bool parse(std::string const & filePath);
	bool save(DirectedGraphImage const & graph);

protected:
	char * stringToChar(std::string const & s);
	char * xmlToChar(std::string const & stageFile);

private:
	bool serializeVertexContext(VertexContext * context, xml_node<> * parentNode, std::string const & childName);
	void serializeColor(QColor const & color, xml_node<> * parentNode, const char * childName);
	void serializeFont(QFont const & font, xml_node<> * parentNode, char const * childName);
};

