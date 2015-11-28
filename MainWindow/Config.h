#pragma once

#include <libconfig.h++>
#include <QTextCodec>
#include <QString>

#include "Singleton.h"
#include "Tool.h"
#include "VertexContext.h"
#include "Tools.h"
#include "Edges.h"
#include "EdgeContext.h"
#include "VertexBuilder.h"

namespace Application
{
	class Config : public Singleton<Config>
	{
		friend class Singleton<Config>;
		DrawingTool * _tool;
		libconfig::Config _cfg;

		VertexContext * _defaultVertexContext;
		VertexContext * _selectedVertexContext;

		EdgeContext * _defaultEdgeContext;
		EdgeContext * _selectedEdgeContext;
		QString _graphStatusString;

		ToolType _currentTool;
		EdgeType _currentEdgeType;
	private:
		Config();
		~Config();

	public:
		inline VertexContext * DefaultVertexContext() const { return _defaultVertexContext; }
		void DefaultVertexContext(VertexContext * const val) { _defaultVertexContext = val; }
		inline VertexContext * SelectedVertexContext() const { return _selectedVertexContext; }
		void SelectedVertexContext(VertexContext * const val) { _selectedVertexContext = val; }
		inline QString GraphStatusString() const { return _graphStatusString; }
		inline EdgeContext * DefaultEdgeContext() const { return _defaultEdgeContext; }
		void DefaultEdgeContext(EdgeContext * const  val) { _defaultEdgeContext = val; }
		inline EdgeContext * SelectedEdgeContext() const { return _selectedEdgeContext; }
		void SelectedEdgeContext(EdgeContext * const val) { _selectedEdgeContext = val; }
		inline ToolType CurrentToolType() const { return _currentTool; }
		void CurrentToolType(ToolType val)
		{
			_currentTool = val;
		}
		inline EdgeType CurrentEdgeType() const { return _currentEdgeType; }
		void CurrentEdgeType(EdgeType val) { _currentEdgeType = val; }
		inline DrawingTool * CurrentTool() const { return _tool; }

		void changeCurrentTool(Tool * tool)
		{
			_tool->changeTool(tool);
		}

		QMap<QString, QString> getAlgorithmMap(std::string const & algo);

	private:
		void ReadVertices(libconfig::Setting const & root);
		void ReadVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext *& context);
		void WriteVertices(libconfig::Setting const & root);
		void WriteVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext * context);

		void ReadEdges(libconfig::Setting const & root);
		void ReadEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext *& context);
		void WriteEdges(libconfig::Setting const & root);
		void WriteEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext * context);

		void ReadAlgorithmMap(libconfig::Setting const & algoritmsNode, QMap<QString, QString> & algoritmMap);
	};
}
