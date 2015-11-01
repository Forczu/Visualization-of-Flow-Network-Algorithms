#pragma once

template<typename T>
class Singleton
{
protected:
	static T * _pInstance;
public:
	static T & Instance()
	{
		static bool __initialized = false;
		if (!__initialized)
		{
			_pInstance = new T();
			atexit(destroy);
			__initialized = true;
		}
		return *_pInstance;
	}
protected:
	Singleton() { }
	static void destroy()
	{
		delete _pInstance;
	}
};

