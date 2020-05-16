#pragma once
#include "QtIncludes.h"
class CollisionManager : public QGroupBox
{
	CollisionManager(QWidget* parent = 0);
	Q_OBJECT
public:
	void AddCollision();
	void UpdateCollision();
	void UpdateSelectedObject(GameObject* selectedObject);
	void SetCollisionType(CollisionType type);

private:
	GameObject* selectedObject = nullptr;


};

