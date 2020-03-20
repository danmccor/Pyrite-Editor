#include "AddAction.h"
#include "qdebug.h"
AddAction::AddAction(QWidget* parent) : QGroupBox(parent)
{
	QPushButton* AddMovement = this->findChild<QPushButton*>("AddMovement");
	if (AddMovement != nullptr) {
		qDebug() << "Button Found \n";
		connect(AddMovement, SIGNAL(AddMovement->clicked()), this, SLOT(addToBox()));
	}
}

void AddAction::addToBox()
{
	
}


