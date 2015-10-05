#pragma once

#include <libconfig.h++>
#include <QTextCodec>
#include <QString>

#include "VertexContext.h"
#include "EdgeContext.h"

namespace Application
{
	class Config
	{
		static Config * _pInstance;

		libconfig::Config _cfg;

		VertexContext _defaultVertexContext;
		VertexContext _selectedVertexContext;

		EdgeContext _defaultEdgeContext;
		EdgeContext _selectedEdgeContext;
		QString _graphStatusString;

		bool _weighted;
		bool _directed;

	public:
		static Config & Instance();
	private:
		Config();
		~Config();

	public:
		inline VertexContext const & DefaultVertexContext() const
		{
			return _defaultVertexContext;
		}
		void DefaultVertexContext(VertexContext const & val)
		{
			_defaultVertexContext = val;
		}
		inline VertexContext const & SelectedVertexContext() const
		{
			return _selectedVertexContext;
		}
		void SelectedVertexContext(VertexContext const & val)
		{
			_selectedVertexContext = val;
		}
		inline QString GraphStatusString() const
		{
			return _graphStatusString;
		}
		inline EdgeContext const & DefaultEdgeContext() const { return _defaultEdgeContext; }
		void DefaultEdgeContext(EdgeContext const &  val) { _defaultEdgeContext = val; }
		inline EdgeContext const & SelectedEdgeContext() const { return _selectedEdgeContext; }
		void SelectedEdgeContext(EdgeContext const & val) { _selectedEdgeContext = val; }
		inline bool IsGraphWeighted() const { return _weighted; }
		void SetGraphWeighted(bool val) { _weighted = val; }
		inline bool IsGraphDirected() const { return _directed; }
		void SetGraphDirected(bool val) { _directed = val; }

	private:
		void ReadVertices(libconfig::Setting const & root);
		void ReadVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext & context);
		void WriteVertices(libconfig::Setting const & root);
		void WriteVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext & context);

		void ReadEdges(libconfig::Setting const & root);
		void ReadEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext & context);
		void WriteEdges(libconfig::Setting const & root);
		void WriteEdgeContext(libconfig::Setting const & edges, std::string const & nodeName, EdgeContext & context);

		void ReadStatusString(libconfig::Setting const & root);
		static void DestroyConfig();
	};
}
