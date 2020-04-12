#pragma once
#include "GameObject.h"

class GameCamera : public GameObject
{
public:
	GameCamera(WindowFramework* window);
	NodePath camera;
	void LoadModel(PandaFramework* framework);
	NodePath cameraNodePath;

	void ActivateCamera();
	NodePath& GetCameraNodePath() { return cameraNodePath; };
};

