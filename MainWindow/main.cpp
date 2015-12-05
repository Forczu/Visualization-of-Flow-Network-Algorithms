#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QTime>

#ifdef DEBUG
#include <vld.h>
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setStyleSheet(QString::fromUtf8(
		"    QScrollBar:vertical {"
		"	border: 0px;"
		"	background: #fff;"
		"	margin: 0px;"
		"	width: 12px;"
		"}"
		"QScrollBar::handle:vertical{"
		"background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,"
		"		stop : 0 #D7D7D7, stop: 1.0 #A2A2A2);"
		"	min - height: 20px;"
		"border: 1px solid #9F9F9F;"
		"	border-radius: 5px;"
		"}"
		"QScrollBar::add-line:vertical {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb(32, 47, 130));"
		"    height: px;"
		"    subcontrol-position: bottom;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:vertical {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb(32, 47, 130));"
		"    height: 0px;"
		"    subcontrol-position: top;"
		"    subcontrol-origin: margin;"
		"}"
		
		"    QScrollBar:horizontal {"
		"	border: 0px;"
		"	background: #fff;"
		"	margin: 0px;"
		"	height: 12px;"
		"}"
		"QScrollBar::handle:horizontal{"
		"background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,"
		"		stop : 0 #D7D7D7, stop: 1.0 #A2A2A2);"
		"	min - width: 24px;"
		"border: 1px solid #9F9F9F;"
		"	border-radius: 5px;"
		"}"
		"QScrollBar::add-line:horizontal {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb(32, 47, 130));"
		"    height: px;"
		"    subcontrol-position: right;"
		"    subcontrol-origin: margin;"
		"}"
		"QScrollBar::sub-line:horizontal {"
		"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
		"    stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb(32, 47, 130));"
		"    height: 0px;"
		"    subcontrol-position: left;"
		"    subcontrol-origin: margin;"
		"}"
		));

	MainWindow w;
	w.show();
	return a.exec();
}
