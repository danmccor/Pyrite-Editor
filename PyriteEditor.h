#pragma once

#include <QtCore/qcoreapplication.h>
#include "ui_PyriteEditor.h"
#include "Panda3D.h"
#include "GameObject.h"
#include "TriggerManager.h"
#include "CollisionManager.h"
#include "TransformManager.h"

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
	TransformManager* transformManager = nullptr;

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

	QWidget* newWindow = nullptr;

	bool updateListOnce = false;
};
