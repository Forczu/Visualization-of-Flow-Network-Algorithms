#include "Config.h"
#include "VertexContext.h"

Application::Config * Application::Config::_pInstance = 0;

namespace Application
{
	Config::Config() : _currentTool(ToolType::None)
	{
		try
		{
			_cfg.readFile("config\\config.cfg");
			libconfig::Setting const & root = _cfg.getRoot();
			ReadVertices(root);
			ReadEdges(root);
		}
		catch (libconfig::FileIOException const & e)
		{
		}
	}

	Config::~Config()
	{
		delete _defaultVertexContext;
		delete _selectedVertexContext;
		delete _defaultEdgeContext;
		delete _selectedEdgeContext;
	}

	QMap<QString, QString> Config::getAlgorithmMap(std::string const & algo)
	{
		libconfig::Setting const & root = _cfg.getRoot();
		libconfig::Setting const & algoritmsNode = root["application"]["algorithms"];
		QMap<QString, QString> algorithmMap;
		ReadAlgorithmMap(algoritmsNode[algo], algorithmMap);
		return algorithmMap;
	}

	void Config::ReadVertices(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["vertices"];
		ReadVertexContext(vertices, "default_vertex", _defaultVertexContext);
		ReadVertexContext(vertices, "selected_vertex", _selectedVertexContext);
	}

	void Config::ReadVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext *& context)
	{
		libconfig::Setting const & df = vertices[nodeName];
		int size = df["size"];
		int strokeSize = df["stroke"];
		libconfig::Setting const & colorNode = df["color"];
		QColor color = QColor(colorNode["r"], colorNode["g"], colorNode["b"]);
		libconfig::Setting const & strokeColorNode = df["color_stroke"];
		QColor strokeColor = QColor(strokeColorNode["r"], strokeColorNode["g"], strokeColorNode["b"]);
		libconfig::Setting const & fontNode = df["font"];
		QFont font;
		font.setBold(static_cast<int>(fontNode["bold"]) == 1 ? true : false);
		font.setPointSize(fontNode["size"]);
		font.setFamily(QString(fontNode["family"]));
		VertexBuilder builder = VertexBuilder(size, strokeSize);
		context = builder.color(color)->strokeColor(strokeColor)->font(font)->build();
	}

	void Config::WriteVertices(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["vertices"];
		WriteVertexContext(vertices, "default_vertex", _defaultVertexContext);
		WriteVertexContext(vertices, "selected_vertex", _selectedVertexContext);
	}

	void Config::WriteVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext * context)
	{
		libconfig::Setting const & df = vertices[nodeName];
		df["size"] = context->Size();
		df["stroke"] = context->StrokeSize();
		libconfig::Setting const & colorNode = df["color"];
		QColor color = context->Color();
		colorNode["r"] = color.red();
		colorNode["g"] = color.green();
		colorNode["b"] = color.blue();
		libconfig::Setting const & strokeColorNode = df["color_stroke"];
		color = context->StrokeColor();
		strokeColorNode["r"] = color.red();
		strokeColorNode["g"] = color.green();
		strokeColorNode["b"] = color.blue();
		_cfg.writeFile("config\\config.cfg");
	}

	void Config::ReadEdges(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["edges"];
		ReadEdgeContext(vertices, "default_edge", _defaultEdgeContext);
		ReadEdgeContext(vertices, "selected_edge", _selectedEdgeContext);
	}

	void Config::ReadEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext *& context)
	{
		libconfig::Setting const & df = edges[nodeName];
		int size = df["size"];
		libconfig::Setting const & colorNode = df["color"];
		QColor color = QColor(colorNode["r"], colorNode["g"], colorNode["b"]);
		context = new EdgeContext(size, color);
	}

	void Config::WriteEdges(libconfig::Setting const & root)
	{
		libconfig::Setting const & edges = root["application"]["edges"];
		WriteEdgeContext(edges, "default_edge", _defaultEdgeContext);
		WriteEdgeContext(edges, "selected_edge", _selectedEdgeContext);
	}

	void Config::WriteEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext * context)
	{
		libconfig::Setting const & df = edges[nodeName];
		df["size"] = context->Size();
		libconfig::Setting const & colorNode = df["color"];
		QColor color = context->Color();
		colorNode["r"] = color.red();
		colorNode["g"] = color.green();
		colorNode["b"] = color.blue();
		_cfg.writeFile("config\\config.cfg");
	}

	void Config::ReadAlgorithmMap(libconfig::Setting const & algoritmsNode, QMap<QString, QString> & algoritmMap)
	{
		if (!algoritmsNode.isGroup())
			return;
		for (auto it = algoritmsNode.begin(); it != algoritmsNode.end(); ++it)
		{
			algoritmMap.insert(QString(it->getName()), QString(*it));
		}
	}

}
