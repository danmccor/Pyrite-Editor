#pragma once
#include <QtWidgets/qpushbutton.h>
#include <QtGui>
#include <QMenu.h>
#include <qfileiconprovider.h>
class AddComponent : public QMenu
{
public:
	explicit AddComponent(QPushButton* button, QWidget* parent = 0);
	void showEvent(QShowEvent* ev);
private:
	QPushButton* Button;

public slots:
	void CreateComponent();
};

