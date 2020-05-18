#include "CollisionManager.h"

CollisionManager::CollisionManager(QWidget* parent) : QGroupBox(parent) {

}

void CollisionManager::Initialise()
{
	//Find the collision dropdown box
	QComboBox* CollisionType = findChild<QComboBox*>("CollisionType");
	//Add the choice of collision
	CollisionType->addItem("Box", QVariant::fromValue(CollisionType::Box));
	CollisionType->addItem("Sphere", QVariant::fromValue(CollisionType::Sphere));
}

void CollisionManager::AddCollision()
{
	//Add collision to selected object
	if (selectedObject != nullptr && !selectedObject->HasCollision()) {
		//Show the collision options
		this->show();
		selectedObject->AddCollision();
		selectedObject->SetCollisionType(CollisionType::Box, false);
	}
}

void CollisionManager::UpdateCollision()
{
	//Set the collision based on what is in the collision box
	QComboBox* CollisionTypeBox = this->findChild<QComboBox*>("CollisionType");
	QCheckBox* CollisionCheckBox = this->findChild<QCheckBox*>("CollisionCheckBox");
	selectedObject->ChangeCollisionType(CollisionType(CollisionTypeBox->currentData().toInt()), CollisionCheckBox->isChecked());
}

void CollisionManager::UpdateSelectedObject(GameObject* selectedObject)
{
	//Update the selected object
	this->selectedObject = selectedObject;
}

void CollisionManager::SetCollisionType(CollisionType type)
{
	findChild<QComboBox*>("CollisionType")->setCurrentIndex(QVariant::fromValue(type).toInt());
	findChild<QCheckBox*>("CollisionCheckBox")->setChecked(selectedObject->GetCollisionPushSetting());
}
