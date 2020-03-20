#pragma once
#include "GameObject.h"
#include "mouseWatcher.h"
#include "mouseAndKeyboard.h"
#include "collisionNode.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "notifyCategory.h"
#include "mouseButton.h"
#include "lvector3.h"

class Panda3D
{
public:
	bool init(size_t hwnd, int argc, char* argv[], int width, int height, int originX, int originY);
	void EnableKeyboard();
	void displayScene();
	void closePanda3D();
	void runLoop();
	void CameraMovement();
	void MouseCollider();

	void createObject(std::string modelLocation);
	GameObject* GetSelectedObject();


	WindowFramework* GetWindow() { return window; };
	PandaFramework framework;

private:
	WindowFramework* window;
	WindowProperties properties;
	std::vector<GameObject> gameObjects;
	
	NodePath camera;

	PT(MouseWatcher) mouseWatcher;
	PT(CollisionRay) collisionRay;
	CollisionTraverser cTrav = CollisionTraverser("cTrav");
	PT(CollisionHandlerQueue) cHandler;
	CollisionNode* collisionRay_Node;
	NodePath collisionRay_NodePath;
	NodePath yupAxis;
	GameObject* selectedObject;

	void MoveCameraForward(const Event* theEvent, void* data);
	void MoveCameraBackward();
	void MoveCameraRight();
	void MoveCameraLeft();
};

