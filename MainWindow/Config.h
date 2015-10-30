#pragma once

#include <libconfig.h++>
#include <QTextCodec>
#include <QString>

#include "VertexContext.h"
#include "Tools.h"
#include "Edges.h"
#include "EdgeContext.h"
#include "VertexBuilder.h"

namespace Application
{
	class Config
	{
		static Config * _pInstance;

		libconfig::Config _cfg;

		VertexContext * _defaultVertexContext;
		VertexContext * _selectedVertexContext;

		EdgeContext * _defaultEdgeContext;
		EdgeContext * _selectedEdgeContext;
		QString _graphStatusString;

		Tool _currentTool;
		EdgeType _currentEdgeType;
	public:
		static Config & Instance();
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
		inline Tool CurrentTool() const { return _currentTool; }
		void CurrentTool(Tool val) { _currentTool = val; }
		inline EdgeType CurrentEdgeType() const { return _currentEdgeType; }
		void CurrentEdgeType(EdgeType val) { _currentEdgeType = val; }

	private:
		void ReadVertices(libconfig::Setting const & root);
		void ReadVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext *& context);
		void WriteVertices(libconfig::Setting const & root);
		void WriteVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext * context);

		void ReadEdges(libconfig::Setting const & root);
		void ReadEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext *& context);
		void WriteEdges(libconfig::Setting const & root);
		void WriteEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext * context);

		void ReadStatusString(libconfig::Setting const & root);
		static void DestroyConfig();
	};
}
