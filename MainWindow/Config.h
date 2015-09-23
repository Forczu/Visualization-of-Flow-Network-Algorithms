#pragma once

#include <libconfig.h++>
#include "VertexContext.h"

namespace Application
{
	class Config
	{
		static Config * _pInstance;

		libconfig::Config _cfg;

		VertexContext _defaultVertexContext;
		VertexContext _selectedVertexContext;

	public:
		static Config & Instance()
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

	private:
		void ReadVertices(libconfig::Setting const & root);
		void ReadVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext & context);
		void WriteVertices(libconfig::Setting const & root);
		void WriteVertexContext(libconfig::Setting const & vertices, std::string const & nodeName, VertexContext & context);

		static void DestroyConfig()
		{
			delete _pInstance;
		}
	};
}
