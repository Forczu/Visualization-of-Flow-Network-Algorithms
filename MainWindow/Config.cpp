#include "Config.h"

namespace Application
{
	Config * Config::_pInstance = 0;

	Config & Config::Instance()
	{
		static bool __initialized = false;
		if (!__initialized)
		{
			_pInstance = new Config;
			atexit(DestroyConfig);
			__initialized = true;
		}
		return *_pInstance;
	}

	Config::Config() : _currentTool(Tool::None)
	{
		try
		{
			_cfg.readFile("Config\\config.cfg");
			libconfig::Setting const & root = _cfg.getRoot();
			ReadVertices(root);
			ReadEdges(root);
			ReadStatusString(root);
		}
		catch (libconfig::FileIOException e)
		{
		}
	}

	Config::~Config()
	{
		libconfig::Setting const & root = _cfg.getRoot();
		WriteVertices(root);
		WriteEdges(root);
	}

	void Config::ReadVertices(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["vertices"];
		ReadVertexContext(vertices, "default_vertex", _defaultVertexContext);
		ReadVertexContext(vertices, "selected_vertex", _selectedVertexContext);
	}

	void Config::ReadVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext & context)
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
		context = VertexContext(size, strokeSize, color, strokeColor, font);
	}

	void Config::WriteVertices(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["vertices"];
		WriteVertexContext(vertices, "default_vertex", _defaultVertexContext);
		WriteVertexContext(vertices, "selected_vertex", _selectedVertexContext);
	}

	void Config::WriteVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext & context)
	{
		libconfig::Setting const & df = vertices[nodeName];
		df["size"] = context.Size();
		df["stroke"] = context.StrokeSize();
		libconfig::Setting const & colorNode = df["color"];
		QColor color = context.Color();
		colorNode["r"] = color.red();
		colorNode["g"] = color.green();
		colorNode["b"] = color.blue();
		libconfig::Setting const & strokeColorNode = df["color_stroke"];
		color = context.StrokeColor();
		strokeColorNode["r"] = color.red();
		strokeColorNode["g"] = color.green();
		strokeColorNode["b"] = color.blue();
		_cfg.writeFile("Config\\config.cfg");
	}

	void Config::ReadEdges(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["edges"];
		ReadEdgeContext(vertices, "default_edge", _defaultEdgeContext);
		ReadEdgeContext(vertices, "selected_edge", _selectedEdgeContext);
	}

	void Config::ReadEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext & context)
	{
		libconfig::Setting const & df = edges[nodeName];
		int size = df["size"];
		libconfig::Setting const & colorNode = df["color"];
		QColor color = QColor(colorNode["r"], colorNode["g"], colorNode["b"]);
		context = EdgeContext(size, color);
	}

	void Config::WriteEdges(libconfig::Setting const & root)
	{
		libconfig::Setting const & edges = root["application"]["edges"];
		WriteEdgeContext(edges, "default_edge", _defaultEdgeContext);
		WriteEdgeContext(edges, "selected_edge", _selectedEdgeContext);
	}

	void Config::WriteEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext & context)
	{
		libconfig::Setting const & df = edges[nodeName];
		df["size"] = context.Size();
		libconfig::Setting const & colorNode = df["color"];
		QColor color = context.Color();
		colorNode["r"] = color.red();
		colorNode["g"] = color.green();
		colorNode["b"] = color.blue();
		_cfg.writeFile("Config\\config.cfg");
	}

	void Config::ReadStatusString(libconfig::Setting const & root)
	{
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1250"));
		libconfig::Setting const & graphNode = root["application"]["graph"];
		std::string tmp = graphNode["status"].c_str();
		_graphStatusString = QString::fromLocal8Bit(graphNode["status"]);
	}

	void Config::DestroyConfig()
	{
		delete _pInstance;
	}
}
