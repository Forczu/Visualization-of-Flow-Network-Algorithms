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
		ReadVerticeContext(vertices, "default_vertice", _defaultVerticeContext);
		ReadVerticeContext(vertices, "selected_vertice", _selectedVerticeContext);
	}

	void Config::ReadVerticeContext(libconfig::Setting const & vertices, std::string const & nodeName, VerticeContext & context)
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
		context = VerticeContext(size, strokeSize, color, strokeColor, font);
	}

	void Config::WriteVertices(libconfig::Setting const & root)
	{
		libconfig::Setting const & vertices = root["application"]["vertices"];
		WriteVerticeContext(vertices, "default_vertice", _defaultVerticeContext);
		WriteVerticeContext(vertices, "selected_vertice", _selectedVerticeContext);
	}

	void Config::WriteVerticeContext(libconfig::Setting const & vertices, std::string const & nodeName, VerticeContext & context)
	{
		libconfig::Setting const & df = vertices[nodeName];
		df["size"] = _defaultVerticeContext.Size();
		df["stroke"] = _defaultVerticeContext.StrokeSize();
		libconfig::Setting const & colorNode = df["color"];
		QColor color = _defaultVerticeContext.Color();
		colorNode["r"] = color.red();
		colorNode["g"] = color.green();
		colorNode["b"] = color.blue();
		libconfig::Setting const & strokeColorNode = df["color_stroke"];
		color = _defaultVerticeContext.StrokeColor();
		strokeColorNode["r"] = color.red();
		strokeColorNode["g"] = color.green();
		strokeColorNode["b"] = color.blue();
		_cfg.writeFile("Config\\config.cfg");
	}
}
