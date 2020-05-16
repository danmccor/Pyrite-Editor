#pragma once
#include "Panda3D.h"
#include "QtIncludes.h"

extern Panda3D pandaEngine;
class TriggerManager : public QGroupBox
{
	Q_OBJECT
public:
	TriggerManager(QWidget* parent = 0);
	void UpdateSelectedObject(GameObject* selectedObject);
	void UpdateDirectory(std::string directory);
	void Initialise(std::string directory);
	void LoadTriggerBoxes();
	void RemoveTriggerBoxes();
	void UpdateActions();

public slots:
	void AddTrigger();
	void AddTriggerAction(int id = 0, int enterID = 0, int selectedObjectID = 0, Action action = Action::Move, Direction direction = Direction::Forward, bool newAction = true);
	void AddTriggerMoveAction(int enterID = 0, int selectedObjectID = 0, LPoint3 position = (0, 0, 0), bool newAction = true);
	void AddTriggerChangeScene(int enterID = 0, int selectedObjectID = 0, std::string newScene = "", bool newAction = true);

private:
	GameObject* selectedObject = nullptr;

	QComboBox* ConnectedObjectBox = nullptr;
	QComboBox* EnteringObjectBox = nullptr;

	std::string ProjectDirectory;
};

