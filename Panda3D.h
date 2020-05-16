#pragma once
#include "GameObject.h"
#include "GameCamera.h"
#include "mouseWatcher.h"
#include "mouseAndKeyboard.h"
#include "collisionNode.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionHandlerPusher.h"
#include "notifyCategory.h"
#include "mouseButton.h"
#include "lvector3.h"
#include "audioManager.h"
#include "WorldGrid.h"

class SaveManager;

class Panda3D
{

//Public Functions
public:
	WorldGrid grid;
	bool Init(size_t hwnd, int argc, char* argv[], int width, int height, int originX, int originY, bool built = false);

	void SetFirstScene(std::string scene);
	std::string GetFirstScene() { return firstScene; };

	void RunLoop();
	void MouseCollider();

	WindowFramework* GetWindow() { return window; };

	GameObject* CreateObject(std::string modelLocation, std::string objectName);
	GameObject* GetSelectedObject();
	void SetSelectedObject(GameObject* object);
	std::vector<GameObject*> GetVectorOfGameObjects();
	GameObject* GetObjectByID(int id);
	void AddGameObject(GameObject* object);
	void RemoveAllGameObjects();
	void DeleteGameObject(int id);

	void AddWorldTrigger();
	void AddGameCamera();
	void AttachCamera(GameObject* camera);

	void ResetHandlers();

	void SetMusic(std::string musicLocation);
	void ClosePanda3D();

//Public Variables
public:
	PandaFramework framework;
	bool readyToLoadScene = false;
	std::string SceneToLoad = "";

//Private Functions
private:
	void CheckObjectTriggers();
	void CheckObjectCollisions();

//Private Variables
private:
	WindowFramework* window;
	std::vector<GameObject*> gameObjects;
	
	NodePath camera;
	PT(MouseWatcher) mouseWatcher;
	PT(CollisionRay) collisionRay;

	CollisionTraverser cTrav = CollisionTraverser("cTrav");
	CollisionTraverser cTriggerTrav = CollisionTraverser("cTrigTrav");

	PT(CollisionHandlerQueue) cHandler;
	PT(CollisionHandlerQueue) cTravHandler;
	PT(CollisionHandlerPusher) cPusher;

	PT(AudioManager) audioManager;
	PT(AudioSound) music;
	std::string musicLocation;

	CollisionNode* collisionRay_Node;
	NodePath collisionRay_NodePath;

	NodePath yupAxis;

	GameObject* selectedObject;

	std::string firstScene = "";
	bool collisionsHandled = false;
	bool triggersHandled = false;

	bool built = false;
};

