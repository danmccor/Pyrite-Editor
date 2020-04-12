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
	camera = window->get_camera_group();
	std::string output = "Number of cameras: " + std::to_string(window->get_num_cameras()) + "\n";
	OutputDebugStringA(output.c_str());

	camera.set_pos(Model.get_pos());
	camera.set_hpr(Model.get_hpr());
	output = "Model Current x: " + std::to_string(Model.get_x()) + " Model Current y: " + std::to_string(Model.get_y()) + " Model Current z: " + std::to_string(Model.get_z());
	OutputDebugStringA(output.c_str());

}
