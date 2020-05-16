#pragma once
#include "QtIncludes.h"
class CollisionManager : public QGroupBox
{
	Q_OBJECT
public:
	CollisionManager(QWidget* parent = 0);
	void Initialise();
	void UpdateCollision();
	void UpdateSelectedObject(GameObject* selectedObject);
	void SetCollisionType(CollisionType type);

public slots:
	void AddCollision();
private:
	GameObject* selectedObject = nullptr;


};

