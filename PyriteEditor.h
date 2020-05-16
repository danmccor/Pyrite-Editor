#pragma once

#include <QtCore/qcoreapplication.h>
#include "ui_PyriteEditor.h"
#include "Panda3D.h"
#include "GameObject.h"
#include "TriggerManager.h"
#include "CollisionManager.h"

extern Panda3D pandaEngine;

class PyriteEditor : public QMainWindow
{
	Q_OBJECT

public:
	//Constructor for the Pyrite Editor
	PyriteEditor(QWidget* parent = Q_NULLPTR);

	//Set the properties for the object passed in. 
	void SetObjectProperties(GameObject* gameObject);

	//Update all Components in the game.
	void UpdateComponents();

	//Load the variables of the selected object.
	void LoadSelectedObject();
	
	//return whether the editor is in edit mode or run mode. 
	bool IsEditing() { return isEditing; };

public slots:
	//Add an action box to the transform component
	void AddActionBox(Action action = Action::Move, std::string key = "", float speed = NULL, Direction direction = Direction::Forward, bool newAction = true);

	//Transform Follow Object
	void AddFollowObject(TransformAxis axis = TransformAxis::X, int selectedObjectID = 0, std::string key = "", float speed =  0, bool newAction = true);

	//Add a trigger action to the trigger component
	/*void AddTriggerAction(int id = 0, int enterID = 0, int selectedObjectID = 0, Action action = Action::Move, Direction direction = Direction::Forward, bool newAction = true);
	void AddTriggerMoveAction(int enterID = 0, int selectedObjectID = 0, LPoint3 position = (0, 0, 0), bool newAction = true);
	void AddTriggerChangeScene(int enterID = 0, int selectedObjectID = 0, std::string newScene = "", bool newAction = true);*/

	void AddObjectToObjectList(GameObject* gameObject);

	//Turn the editor into run mode
	void RunGame();
	//Add a world trigger object to the scene
	void AddWorldTrigger();
	//Add a camera to the scene
	void AddCamera();

	void Test() { qDebug() << "The test has worked"; }

	void DeleteObject();

	void LaunchBuildWindow();

	//Add a transform to the selected object
	void AddTransform();
	//Add collision to the selected object
	void AddCollision();
	//Add a trigger to the selected object
	//void AddTrigger();


	void NewProject();
	void LoadProject();

	//Create a new project
	void New();
	//Save the project as 
	void SaveAs();
	//Save the project(if the current scene is not an existing project, save as will be run)
	void Save();
	//Load a project
	void Load();
	//Build the project to the directory
	void Build();

private slots:
	void SelectObjectFromList(QListWidgetItem* item);

private:
	TriggerManager* triggerManager = nullptr;
	CollisionManager* collisionManager = nullptr;

	//Load the triggers attached to an object to the editor
	//void LoadTriggerBoxes();
	//Remove the trigger boxes attached to an object from the editor
	//void RemoveTriggerBoxes();
	//Load the transforms attached to an object to the editor
	void RemoveActionBoxes();
	//Remove the transform boxes attached to an object from the editor
	void LoadActionBoxes();
	//Set the properties of a loaded object
	void SetObjectProperties(float posX = 0, float posY = 0, float posZ = 0, float rotX = 0, float rotY = 0, float rotZ = 0, float scale = 0);

	void RemoveObjectFromList(GameObject* object);
	void AddAllObjectsToList();

	void ReloadGameObjectDropDowns(QComboBox* comboBox);

	void ConnectUI();

private:
	//The ui for the window.
	Ui::PyriteEngineClass ui;

	//Track last collision on object, prevent collision being updated each frame
	int lastCollisionType = 1;
	
	//Track whether game is in edit mode or not.
	bool isEditing = true;

	//The selected object
	GameObject* selectedObject;

	//Prevent properties updating every frame
	bool oneTimeSelect = true;

	//The save file for the current user project
	QJsonObject ProjectSave;

	//The directory of the project
	std::string ProjectDirectory = "";
	//The project name
	std::string ProjectName = "";
	//The save manager which saves and loads the project. 
	//SaveManager* saveManager = nullptr;

	QWidget* newWindow = nullptr;

	bool updateListOnce = false;
};
