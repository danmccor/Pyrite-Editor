#include "GameCamera.h"

GameCamera::GameCamera(WindowFramework* window) {
	//Set Camera
	ModelLocation = "camera.egg";
	ObjectName = "Camera";
	id = objectCount;
	this->window = window;
	objectCount++;
	isCamera = true;
}

void GameCamera::LoadModel(PandaFramework* framework)
{
	Model = window->load_model(framework->get_models(), ModelLocation);
	Model.reparent_to(window->get_render());
	AttachEditorCollider();
	AddTriggerInteractor();
}

void GameCamera::ActivateCamera()
{
	//Activate the camera by setting default camera to camera position
	camera = window->get_camera_group();
	camera.set_pos(Model.get_pos());
	camera.set_hpr(Model.get_hpr());
}
