#include "CollisionManager.h"

CollisionManager::CollisionManager(QWidget* parent) : QGroupBox(parent) {

}

void CollisionManager::AddCollision() 
{
	selectedObject->AddCollision();
}

void CollisionManager::UpdateCollision()
{
	QComboBox* CollisionTypeBox = this->findChild<QComboBox*>("CollisionType");
	QCheckBox* CollisionCheckBox = this->findChild<QCheckBox*>("CollisionCheckBox");
	selectedObject->ChangeCollisionType(CollisionType(CollisionTypeBox->currentData().toInt()), CollisionCheckBox->isChecked());
}

void CollisionManager::UpdateSelectedObject(GameObject* selectedObject)
{
	this->selectedObject = selectedObject;
}

void CollisionManager::SetCollisionType(CollisionType type)
{
	findChild<QComboBox*>("CollisionType")->setCurrentIndex(QVariant::fromValue(type).toInt());
	findChild<QCheckBox*>("CollisionCheckBox")->setChecked(selectedObject->GetCollisionPushSetting());
}
