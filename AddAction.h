#pragma once
#include "QtWidgets/qpushbutton.h"
#include "QtWidgets/qgroupbox.h"
#include "QtWidgets/qlayout.h"
class AddAction : public QGroupBox
{
public:
	AddAction(QWidget * parent = 0);
public slots:
	void addToBox();
private:
	QGroupBox* groupBox;
};

