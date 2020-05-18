#include "AddComponent.h"

AddComponent::AddComponent(QPushButton* button, QWidget* parent) : QMenu(parent), Button(button)
{

}

void AddComponent::showEvent(QShowEvent* ev)
{
	//Show Button Menu
	QPoint p = this->pos();
	QRect geo = Button->geometry();
	this->move(p.x() + geo.width() - this->geometry().width(), p.y());
}

void AddComponent::CreateComponent()
{
	Button->hide();
}
