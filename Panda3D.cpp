#include "Panda3D.h"
#include "SaveManager.h"

extern SaveManager* saveManager;

#pragma region Panda Start/Close
bool Panda3D::Init(size_t hwnd, int argc, char* argv[], int width, int height, int originX, int originY, bool built)
{

	//Open the panda3D framework
	framework.open_framework(argc, argv);
	//Create a windows properties variable
	WindowProperties properties;
	//Retrieve the default window properties
	framework.get_default_window_props(properties);
	//Set the parent window to the handle
	properties.set_parent_window(hwnd);
	//Set the size of window to the width and height of the handle window
	properties.set_size(width, height);
	//Set the origin to the handles origin
	properties.set_origin(originX, originY);
	//Create the panda3D window using properties
	window = framework.open_window(properties, 0);
	//4window->setup_trackball();
	window->enable_keyboard();

	//Create input watcher (MouseWatch takes input from mouse and keyboard)
	mouseWatcher = DCAST(MouseWatcher, window->get_mouse().node());

	//Create transform model
	yupAxis = window->load_model(framework.get_models(), "models/zup-axis");
	yupAxis.reparent_to(window->get_render());
	yupAxis.hide();

	//Setup collision handlers
	cHandler = new CollisionHandlerQueue();
	cTravHandler = new CollisionHandlerQueue();
	cPusher = new CollisionHandlerPusher();
	audioManager = AudioManager::create_AudioManager();
	saveManager = new SaveManager(nullptr);

	//If this is the editor version
	if (!built) {
		

		//Get the camera
		camera = window->get_camera_group();
		//Set the position back
		camera.set_pos(0, -10, 3);
		camera.set_hpr(0, -18, 0);

		//Create a raycast from the camera to detect objects.
		collisionRay_Node = new CollisionNode("mouseRay");
		collisionRay_NodePath = camera.attach_new_node(collisionRay_Node);
		collisionRay_Node->set_collide_mask(BitMask32::bit(1));
		collisionRay = new CollisionRay();
		collisionRay_Node->add_solid(collisionRay);
		collisionRay_NodePath.show();
		cTrav.add_collider(collisionRay_NodePath, cHandler);
		NodePath render = window->get_render();
		cTrav.show_collisions(render);

		render.attach_new_node(grid.Create());
	}
	//else set built to true;
	else {
		this->built = built;
	}
	return true;
}

void Panda3D::SetFirstScene(std::string scene)
{
	std::string output = "first scene is: " + scene + "\n";
	OutputDebugStringA(output.c_str());
	firstScene = scene;
}

//Shut down panda3d
void Panda3D::ClosePanda3D()
{
	//Close the panda window and the framework
	framework.close_all_windows();
	framework.close_framework();
}
#pragma endregion

#pragma region Game Loop
//Run the panda3d loop
void Panda3D::RunLoop()
{
	if (music != nullptr) {
		if (music->status() == AudioSound::SoundStatus::READY) {
			music->play();
		}
		if (music->status() == AudioSound::SoundStatus::BAD) {
			std::string output = "Music is bad! \n";
			OutputDebugStringA(output.c_str());
		}
	}
	//Loop through gameobjects and run their components
	for (int i = 0; i < gameObjects.size(); i++) {
		if (audioManager != nullptr) {
			audioManager->update();
		}
		CheckObjectCollisions();

		CheckObjectTriggers();

		gameObjects[i]->Run(mouseWatcher);

		if (gameObjects[i]->HasTransform()) {

			for (int j = 0; j < gameObjects[i]->GetNumberOfActions(); j++) {

				if (gameObjects[i]->GetTransformAction(j).type == TransformType::Follow) {

					gameObjects[i]->TransformFollowObject(j, gameObjects[gameObjects[i]->GetTransformAction(j).ConnectedObject], mouseWatcher);
				}
			}
		}
	}
}

//Look at mouse and keyboard input
void Panda3D::MouseCollider()
{
	//Get the mouse position
	static float mouseX = mouseWatcher->get_mouse_x();
	static float mouseY = mouseWatcher->get_mouse_y();
	
	//if there is a selected object
	if (selectedObject != nullptr) {
		//Set the axis to objects position
		yupAxis.set_pos(selectedObject->GetModelNodePath().get_pos());
	}

	//if the mouse is not on the scene
	if (!mouseWatcher->has_mouse()) {
		//Return out of function
		return;
	}
	static int clickedObject = 0;
	static bool mousePressed = false;
	//Get the mouse position
	LPoint2 mpos = mouseWatcher->get_mouse();
	if (!built) {
		//Send ray from camera, with the mouse coordinates
		collisionRay->set_from_lens(window->get_camera(0), mpos);
		//Check any collisions with mouse ray
		cTrav.traverse(window->get_render());
		//If there is collisions


		if (!mouseWatcher->is_button_down(MouseButton::one())) {
			mousePressed = false;
		}

		if (cHandler->get_num_entries() > 0) {
			if (clickedObject > cHandler->get_num_entries() - 1) { clickedObject = 0; }
			//Sort the entries
			cHandler->sort_entries();
			//Get the tag from the object as an int
			int i = atoi(cHandler->get_entry(clickedObject)->get_into_node()->get_tag("Object").c_str());
			//If the left mouse button is pressed
			if (mouseWatcher->is_button_down(MouseButton::one()) && !mousePressed) {
				mousePressed = true;
				//If the selected object is not null
				if (selectedObject != nullptr) {
					//Toggle the highlight on the object
					selectedObject->ToggleHighlight();
				}
				//Set selected object to the object with the tag
				selectedObject = gameObjects[i];
				//Toggle the highlight of the object
				selectedObject->ToggleHighlight();
				//Set the position of the axis
				yupAxis.set_pos(selectedObject->GetModelNodePath().get_pos());
				//Show the axis
				yupAxis.show();
				clickedObject++;
			}
		}
		//if there is no collisions
		else {
			//if the left mouse button is pressed
			if (mouseWatcher->is_button_down(MouseButton::one())) {
				//If the selected object is not null
				if (selectedObject != nullptr) {
					//Toggle the objects highlight
					selectedObject->ToggleHighlight();
				}
				//Hide the transform axis
				yupAxis.hide();
				//Set selected object to nullptr
				selectedObject = nullptr;
			}
		}
	}
	//Create bool to check if this is first right click
	static bool InitialRightClick = true;
	//if right mouse button is down
	if (mouseWatcher->is_button_down(MouseButton::three())) {
		//if it is not the first right click
		if (!InitialRightClick) {
			//set the heading and pitch
			static float heading = 0.0f;
			static float pitch = -18.0f;

			//Get the mouse coordinates
			double mouseX = window->get_graphics_window()->get_pointer(0).get_x();
			double mouseY = window->get_graphics_window()->get_pointer(0).get_y();

			//Take away the half the window size
			mouseX -= window->get_graphics_window()->get_x_size() / 2;
			mouseY -= window->get_graphics_window()->get_y_size() / 2;

			//remove the mouse x and y from the heading and pitch
			heading -= mouseX / 10;
			pitch -= mouseY / 10;

			//Set the cameras rotation
			camera.set_hpr(heading, pitch, 0);
			LVecBase3f forward = camera.get_net_transform()->get_mat().get_row3(1);
			LVecBase3f up = camera.get_net_transform()->get_mat().get_row3(2);
			LVecBase3f right = cross(forward, up);
			//If keyboard w is pressed
			if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('w'))) {
				//Normalise forward
				forward.normalize();
				//Set the cameras position 
				camera.set_pos(camera.get_pos() + (forward / 5));
			}
			//If keyboard s is pressed
			if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('s'))) {
				//Normalise forward
				forward.normalize();
				//Remove the forward from camera position
				camera.set_pos(camera.get_pos() - (forward / 5));
			}
			if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('d'))) {
				//Normalise Right
				right.normalize();
				//Add the right to the cameras position
				camera.set_pos(camera.get_pos() + (right/ 5));
			}
			if (mouseWatcher->is_button_down(KeyboardButton::ascii_key('a'))) {
				//Normalise the right
				right.normalize();
				//Take away the right from the cameras position.
				camera.set_pos(camera.get_pos() - (right / 5));
			}
		}
		//Get the mouse and move it to the centre of the screen
		window->get_graphics_window()->move_pointer(0, window->get_graphics_window()->get_x_size() / 2, window->get_graphics_window()->get_y_size() / 2);
		InitialRightClick = false;
	}
	else {
		//initial right click is true
		InitialRightClick = true;
	}

}
#pragma endregion

#pragma region GameObject Manipulation
//Create a gameobject with a model
GameObject* Panda3D::CreateObject(std::string modelLocation, std::string objectName)
{
	//Create new gameobject
	GameObject* gameObject = new GameObject(window, modelLocation, objectName);
	//Load the model
	gameObject->LoadModel(&framework);
	//Push the model back to the vector
	gameObjects.push_back(gameObject);
	return gameObject;
}

//Add a game object to the vector of objects
void Panda3D::AddGameObject(GameObject* object)
{
	gameObjects.push_back(object);
}

//Delete all game objects
void Panda3D::RemoveAllGameObjects()
{
	//Loop through the game objects vector
	for (int i = 0; i < gameObjects.size(); i++) {
		//Set the static int, objectcount, to 0
		//delete all game objects
		gameObjects[i]->Delete();
	}
	//Clear the gameobjects
	gameObjects.clear();
	//resize the vector to 0.
	gameObjects.resize(0);
}

void Panda3D::DeleteGameObject(int id)
{
	gameObjects[id]->Delete();
	gameObjects.erase(gameObjects.begin()+id);

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->id = i;
		gameObjects[i]->ResetTag();
	}
}

//Get the selected object
GameObject* Panda3D::GetSelectedObject()
{
	//return selected object
	return selectedObject;
}

void Panda3D::SetSelectedObject(GameObject* object)
{
	selectedObject = object;
}

//Get the vector of gameObjects
std::vector<GameObject*> Panda3D::GetVectorOfGameObjects()
{
	return gameObjects;
}
#pragma endregion

#pragma region Checkers
//Check the objects collision
void Panda3D::CheckObjectCollisions()
{
	//Check if this is the first loop
	if (!collisionsHandled)
	{
		//loop through all game objects
		for (int i = 0; i < gameObjects.size(); i++) {
			//If the gameobject has collision
			if (gameObjects[i]->HasCollision()) {
				if (gameObjects[i]->GetCollisionPushSetting()) {
					//Add the collider to the Pusher handler
					cPusher->add_collider(gameObjects[i]->GetColNodePath(), gameObjects[i]->GetModelNodePath());
					//Add the collision to the traverser
					cTrav.add_collider(gameObjects[i]->GetColNodePath(), cPusher);
				}
			}

		}
		//Set first loop to false
		collisionsHandled = false;
	}
	cTrav.traverse(window->get_render());
}

//Check the trigger objects
void Panda3D::CheckObjectTriggers()
{
	//Only allow triggers to be added once
	if (!triggersHandled) {
		//Loop through all game objects
		for (int i = 0; i < gameObjects.size(); i++) {
			//If the gameobject has a trigger
			if (gameObjects[i]->HasTrigger()) {
				//Add the trigger to the traverser
				cTriggerTrav.add_collider(gameObjects[i]->GetTriggerNodePath(), cTravHandler);
			}
		}
		triggersHandled = true;
	}
	//Traverse triggers collisions
	cTriggerTrav.traverse(window->get_render());
	//if there are collisions
	if (cTravHandler->get_num_entries() > 0) {
		//Get the object tag of trigger interaction
		//LOOP THROUGH ENTRIES, NEED TO CHECK ALL INTERACTIONS
		for (int o = 0; o < cTravHandler->get_num_entries(); o++) {
			int i = atoi(cTravHandler->get_entry(o)->get_from_node()->get_tag("TriggerBox").c_str());
			int e = atoi(cTravHandler->get_entry(o)->get_into_node()->get_tag("TriggerBox").c_str());
			//Run through the colliders trigger actions
			for (int j = 0; j < gameObjects[i]->GetNumberOfTriggerActions(); j++) {
				//Get the trigger action
				TriggerActions triggerActions = gameObjects[i]->GetTriggerAction(j);
				//Change the connected objects transform action
				if (e == triggerActions.enteringObjectID) {
					if (triggerActions.type == TriggerType::Change) {
						gameObjects[triggerActions.connectedObjectID]->ChangeTransformAction(
							triggerActions.actionID,
							Action(triggerActions.newAction),
							gameObjects[triggerActions.connectedObjectID]->GetTransformAction(triggerActions.actionID).Key,
							gameObjects[triggerActions.connectedObjectID]->GetTransformAction(triggerActions.actionID).Speed,
							Direction(triggerActions.newDirection)
						);
					}
					if (triggerActions.type == TriggerType::MoveTo) {
						gameObjects[triggerActions.connectedObjectID]->SetPosition(triggerActions.toPos.get_x(), triggerActions.toPos.get_y(), triggerActions.toPos.get_z());
					}
					if (triggerActions.type == TriggerType::Scene) {
						RemoveAllGameObjects();
						saveManager->Load(triggerActions.newScene);
						break;
					}
				}
			}
		}
	}
}
#pragma endregion

#pragma region Add to Scene
//Add trigger to the scene
void Panda3D::AddWorldTrigger()
{
	//Create new game object as trigger
	GameObject* triggerBox = new GameObject(window, TriggerShape::Box);
	//Add the gameobject to the vector of gameobjects
	gameObjects.push_back(triggerBox);
}

//Add a game camera to the scene
void Panda3D::AddGameCamera()
{
	//Create a gamecamera (polymorphism)
	GameObject* camera = new GameCamera(window);
	//Load the camera model
	camera->LoadModel(&framework);
	//Push back the camera
	gameObjects.push_back(camera);
}

void Panda3D::AttachCamera(GameObject* camera)
{
	this->camera = camera->GetCameraNodePath();
}

void Panda3D::ResetHandlers()
{
	collisionsHandled = false;
	triggersHandled = false;
}

void Panda3D::SetMusic(std::string musicLocation)
{
	/*CString path(musicLocation.c_str());
	path.Truncate(path.ReverseFind('//'));
	std::string filePath = path.GetString();*/
	this->musicLocation = musicLocation;
	music = audioManager->get_sound(musicLocation);
}
#pragma endregion