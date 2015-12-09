#include "mainwindow.h"
#include <QTextStream>
#include "Application.h"

#ifdef DEBUG
#include <vld.h>
#endif

QString loadScrollbarStyle();

int main(int argc, char *argv[])
{
	Application a(argc, argv);
	a.setStyleSheet(loadScrollbarStyle());
	MainWindow w;
	w.show();
	a.exec();
}

QString loadScrollbarStyle()
{
	QFile file("config\\scrollbar.txt");
	QTextStream in(&file);
	QString scrollbarStyle;
	if (file.open(QIODevice::ReadOnly))
	{
		while (!in.atEnd())
			scrollbarStyle += in.readLine();
		file.close();
	}
	return scrollbarStyle;
}
