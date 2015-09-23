#include "Config.h"

namespace Application
{
	Config * Config::_pInstance = 0;

	Config::Config()
	{
		try
		{
			_cfg.readFile("Config\\config.cfg");
			libconfig::Setting const & root = _cfg.getRoot();
			ReadVertices(root);
		}
		catch (libconfig::FileIOException e)
		{
		}
	}

	Config::~Config()
	{
		libconfig::Setting const & root = _cfg.getRoot();
		WriteVertices(root);
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
		df["size"] = _defaultVertexContext.Size();
		df["stroke"] = _defaultVertexContext.StrokeSize();
		libconfig::Setting const & colorNode = df["color"];
		QColor color = _defaultVertexContext.Color();
		colorNode["r"] = color.red();
		colorNode["g"] = color.green();
		colorNode["b"] = color.blue();
		libconfig::Setting const & strokeColorNode = df["color_stroke"];
		color = _defaultVertexContext.StrokeColor();
		strokeColorNode["r"] = color.red();
		strokeColorNode["g"] = color.green();
		strokeColorNode["b"] = color.blue();
		_cfg.writeFile("Config\\config.cfg");
	}
}
