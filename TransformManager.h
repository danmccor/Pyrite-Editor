#pragma once
#include "QtIncludes.h"
#include "SaveManager.h"

class TransformManager : public QGroupBox
{
	Q_OBJECT
public:
	TransformManager(QWidget* parent);
	void Initialise();
	void RemoveActionBoxes();
	void LoadActionBoxes();
	void UpdateSelectedObject(GameObject* object);
	void UpdateActions();

public slots:
	void AddTransform();
	void AddActionBox(Action action = Action::Move, std::string key = "", float speed = NULL, Direction direction = Direction::Forward, bool newAction = true);
	void AddFollowObject(TransformAxis axis = TransformAxis::X, int selectedObjectID = 0, std::string key = "", float speed = 0, bool newAction = true);

private:
	GameObject* selectedObject = nullptr;
};

