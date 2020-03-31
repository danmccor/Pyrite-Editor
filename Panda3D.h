#pragma once
#include "GameObject.h"
#include "mouseWatcher.h"
#include "mouseAndKeyboard.h"
#include "collisionNode.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionHandlerPusher.h"
#include "notifyCategory.h"
#include "mouseButton.h"
#include "lvector3.h"

class Panda3D
{
public:
	bool init(size_t hwnd, int argc, char* argv[], int width, int height, int originX, int originY);
	void closePanda3D();
	void runLoop();
	void CameraMovement();
	void MouseCollider();

	void AddCollider(NodePath collider);

	void createObject(std::string modelLocation);
	GameObject* GetSelectedObject();
	void CheckObjectCollisions();
	void CheckObjectTriggers();

	void AddWorldTrigger();

	WindowFramework* GetWindow() { return window; };
	PandaFramework framework;

	std::vector<GameObject> GetVectorOfGameObjects();
	void addGameObject(GameObject object);

private:
	WindowFramework* window;
	WindowProperties properties;
	std::vector<GameObject> gameObjects;
	
	NodePath camera;

	PT(MouseWatcher) mouseWatcher;
	PT(CollisionRay) collisionRay;
	CollisionTraverser cTrav = CollisionTraverser("cTrav");
	CollisionTraverser cTriggerTrav = CollisionTraverser("cTrigTrav");
	PT(CollisionHandlerQueue) cHandler;
	PT(CollisionHandlerQueue) cTravHandler;
	PT(CollisionHandlerPusher) cPusher;
	CollisionNode* collisionRay_Node;
	NodePath collisionRay_NodePath;
	NodePath yupAxis;
	GameObject* selectedObject;
	std::string ProjectDirectory;
};

