#ifndef FLOWNETWORKALGORITHMWINDOW_H
#define FLOWNETWORKALGORITHMWINDOW_H

#include <QWidget>
#include "ui_FlowNetworkAlgorithmWindow.h"

class FlowNetworkAlgorithmWindow : public QWidget
{
	Q_OBJECT

public:
	FlowNetworkAlgorithmWindow(QWidget *parent = 0);
	~FlowNetworkAlgorithmWindow();

private:
	Ui::FlowNetworkAlgorithmWindow ui;
};

#endif // FLOWNETWORKALGORITHMWINDOW_H
