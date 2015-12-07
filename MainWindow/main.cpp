#include "mainwindow.h"
#include <QTextStream>

#ifdef DEBUG
#include <vld.h>
#endif

QString loadScrollbarStyle();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyleSheet(loadScrollbarStyle());
	MainWindow w;
	w.show();
	return a.exec();
}

QString loadScrollbarStyle()
{
	QFile file("Config\\scrollbar.txt");
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
