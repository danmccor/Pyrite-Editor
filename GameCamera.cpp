#include "GameCamera.h"

GameCamera::GameCamera(WindowFramework* window) {
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
	camera = window->make_camera();
	camera.set_pos(Model.get_pos());
	camera.set_hpr(Model.get_hpr());
	Model.remove_node();
	camera.reparent_to(window->get_render());
}
