#pragma once
#include <QApplication>

class Application : public QApplication
{
public:
	Application(int argc, char *argv[]) : QApplication(argc, argv) { }
	virtual ~Application() { }

protected:
	virtual bool notify(QObject * receiver, QEvent *  event) override
	{
		try
		{
			return QApplication::notify(receiver, event);
		}
		catch (...)
		{
			return false;
		}
	}
};