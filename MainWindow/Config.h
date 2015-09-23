#pragma once

#include <libconfig.h++>
#include "VerticeContext.h"

namespace Application
{
	class Config
	{
		static Config * _pInstance;

		libconfig::Config _cfg;

		VerticeContext _defaultVerticeContext;
		VerticeContext _selectedVerticeContext;

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
		inline VerticeContext const & DefaultVerticeContext() const
		{
			return _defaultVerticeContext;
		}
		void DefaultVerticeContext(VerticeContext const & val)
		{
			_defaultVerticeContext = val;
		}
		inline VerticeContext const & SelectedVerticeContext() const
		{
			return _selectedVerticeContext;
		}
		void SelectedVerticeContext(VerticeContext const & val)
		{
			_selectedVerticeContext = val;
		}

	private:
		void ReadVertices(libconfig::Setting const & root);
		void ReadVerticeContext(libconfig::Setting const & vertices, std::string const & nodeName, VerticeContext & context);
		void WriteVertices(libconfig::Setting const & root);
		void WriteVerticeContext(libconfig::Setting const & vertices, std::string const & nodeName, VerticeContext & context);

		static void DestroyConfig()
		{
			delete _pInstance;
		}
	};
}
