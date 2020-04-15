#include "GameObject.h"
#include <ostream>

#pragma region Constructors
int GameObject::objectCount = 0;
//Standard gameObject contruction
GameObject::GameObject(WindowFramework* window, std::string modelLocation)
{
	//Set the id of the object to number of existing objects
	id = objectCount;
	//Get the window, model location and name
	this->window = window;
	this->ModelLocation = modelLocation;
	ObjectName = modelLocation;

	//Increate object count
	objectCount++;
}
//Trigger game object construction
GameObject::GameObject(WindowFramework* window, TriggerShape triggerShape)
{
	//Set id of object
	id = objectCount;
	objectCount++;
	//Get window and set name
	this->window = window;
	ObjectName = "Trigger Box";
	//Create trigger
	trigger = new Trigger();
	//Add the trigger interactor
	AddTriggerInteractor();
	//Load the trigger box
	LoadTriggerBox();
}

//Be able to compare the classes (Does gameobject == gameobject)
bool GameObject::operator==(const GameObject& rhs)
{
	if (id == rhs.id) {
		return true;
	}
	return false;
}
bool GameObject::operator!=(const GameObject& rhs)
{
	if (id == rhs.id) {
		return false;
	}
	return true;
}
#pragma endregion

#pragma region Miscellaneous
//Load the model associated with the gameobject
void GameObject::LoadModel(PandaFramework* framework)
{
	//Load the object
	Model = window->load_model(framework->get_models(), ModelLocation);
	//Reparent the model to the renderer
	Model.reparent_to(window->get_render());
	//Attach trigger interactor
	AttachEditorCollider();
	AddTriggerInteractor();
}

//Run the transform component
void GameObject::Run(MouseWatcher* mouseWatcher)
{
	//If the object has a transform
	if (transform != nullptr) {
		//Run the transform actions
		Model.set_fluid_pos(transform->Move(Model.get_pos(), Model.get_net_transform()->get_mat().get_row3(1), Model.get_net_transform()->get_mat().get_row3(2), mouseWatcher));
		Model.set_hpr(transform->Rotate(Model.get_hpr(), mouseWatcher));
	}
}


//Attach collider to the object so the user can select an object
void GameObject::AttachEditorCollider()
{
	//Create a collision box around the object
	PT(BoundingSphere) boundingSphere = DCAST(BoundingSphere, Model.get_bounds());
	PT(CollisionSphere) gameObject_Solid = new CollisionSphere(boundingSphere->get_center(), boundingSphere->get_radius());
	std::ostringstream tag;
	tag << id;
	PT(CollisionNode) gameObject_Node = new CollisionNode("Sphere");
	gameObject_Node->add_solid(gameObject_Solid);
	gameObject_Node->set_into_collide_mask(BitMask32::bit(1));
	gameObject_Node->set_tag("Object", tag.str());
	gameObject_nodePath = Model.attach_new_node(gameObject_Node);
}
//Activate the object camera
void GameObject::ActivateCamera()
{
	std::string output = "This is not a camera! \n";
}

NodePath& GameObject::GetCameraNodePath()
{
	std::string output = "This is not a camera! \n";
}

//Delete game Objects
void GameObject::Delete()
{
	objectCount--;
	//Check the components, if they exist, remove them
	if (HasTrigger()) {
		TriggerNodePath.remove_node();
	}
	if (HasCollision()) {
		collision->i--;
		CollisionNodePath.remove_node();
	}
	//Remove the model
	Model.remove_node();
}
void GameObject::ResetTag()
{
	std::ostringstream tag;
	tag << id;
	gameObject_nodePath.node()->set_tag("Object", tag.str());
	TriggerNodePath.node()->set_tag("TriggerBox", tag.str());
	if (HasCollision()) {
		CollisionNodePath.node()->set_tag("Object", tag.str());
	}
}
//Change the highlight around an object
void GameObject::ToggleHighlight()
{
	//If the object is already highlighted
	if (highlighted) {
		//Hide the highlight
		gameObject_nodePath.hide();
		highlighted = false;
	}
	//if it is not highlighted
	else {
		//highlight the object
		gameObject_nodePath.show();
		highlighted = true;
	}
}
#pragma endregion

#pragma region Object Variables (Pos, rot, etc)
//Set positions, rotation and scale
void GameObject::SetPosition(float x, float y, float z)
{
	Model.set_pos(x, y, z);
}
void GameObject::SetRotation(float x, float y, float z)
{
	Model.set_hpr(x, y, z);
}
void GameObject::SetScale(float x, float y, float z)
{
	Model.set_scale(x, y, z);
}
//Get the object position, rotation and scale
void GameObject::ChangePosition(float x, float y, float z)
{
	Model.set_pos(Model.get_pos().get_x() + x, Model.get_pos().get_y() + y, Model.get_pos().get_z() + z);
}
float GameObject::GetPositionX()
{
	return Model.get_pos().get_x();
}
float GameObject::GetPositionY()
{
	return Model.get_pos().get_y();
}
float GameObject::GetPositionZ()
{
	return Model.get_pos().get_z();
}
float GameObject::GetRotationX()
{
	return Model.get_h();
}
float GameObject::GetRotationY()
{
	return Model.get_p();
}
float GameObject::GetRotationZ()
{
	return Model.get_r();
}
float GameObject::GetScaleX() {
	return Model.get_scale().get_x();
}
float GameObject::GetScaleY() {
	return Model.get_scale().get_y();
}
float GameObject::GetScaleZ() {
	return Model.get_scale().get_z();
}
float GameObject::GetScale()
{
	return Model.get_scale().get_x();
}

//Set/Get Object name
void GameObject::SetObjectName(std::string name)
{
	ObjectName = name;
}
std::string GameObject::GetObjectName()
{
	return ObjectName;
}

//Set/Get the model file location
void GameObject::SetFileLocation(std::string location)
{
	ModelLocation = location;
}
std::string GameObject::GetFileLocation()
{
	return ModelLocation;
}
#pragma endregion

#pragma region NodePaths
//Get Node Paths
NodePath& GameObject::GetTriggerNodePath()
{
	return TriggerNodePath;
}
NodePath& GameObject::GetColNodePath()
{
	return CollisionNodePath;
}
NodePath& GameObject::GetModelNodePath()
{
	return Model;
}
#pragma endregion

#pragma region Transform Component
//Add and change the transform component
void GameObject::AddTransform()
{
	transform = new Transform();
}
bool GameObject::HasTransform()
{
	if (transform != nullptr) {
		return true;
	}
	return false;
}
void GameObject::AddTransformAction(Action action, std::string key, float speed, Direction direction)
{
	transform->AddAction(action, speed, direction, key);
}

void GameObject::ChangeTransformAction(int id, Action action, std::string key, float speed, Direction direction)
{
	transform->GetAction(id).action = action;
	transform->GetAction(id).Key = key;
	transform->GetAction(id).Speed = speed;
	transform->GetAction(id).direction = direction;

}
void GameObject::AddTranformFollowAction(TransformAxis axis, int selectedObject, std::string key, float speed)
{
	transform->AddFollowAction(axis, selectedObject, key, speed);
}
void GameObject::ChangeTransformFollowAction(int id, TransformAxis axis, int selectedObject, std::string key, float speed)
{
	transform->GetAction(id).axis = axis;
	transform->GetAction(id).ConnectedObject = selectedObject;
	std::string output = "Inside change Follow Action: " + std::to_string(selectedObject) + "\n";
	OutputDebugStringA(output.c_str());
	transform->GetAction(id).Key = key;
	transform->GetAction(id).Speed = speed;

}
TransformAction& GameObject::GetTransformAction(int id)
{
	return transform->GetAction(id);
}
void GameObject::TransformFollowObject(int id, GameObject* object, MouseWatcher* mouseWatcher)
{
	Model.set_pos(transform->FollowMove(id, Model.get_pos(), object->GetModelNodePath().get_pos(), mouseWatcher));
}
#pragma endregion

#pragma region Collision Component
//Add and change the object collision
void GameObject::AddCollision()
{

	LPoint3 scale = Model.get_scale();
	LPoint3 pos = Model.get_pos();
	Model.set_pos(0, 0, 0);
	Model.set_scale(1, 1, 1);
	//TriggerNodePath.detach_node();
	collision = new Collision(&Model);
	//Model.attach_new_node(TriggerNodePath.node());
	Model.set_scale(scale);
	Model.set_pos(pos);
}
bool GameObject::HasCollision()
{
	if (collision != nullptr) {
		return true;
	}
	return false;
}
void GameObject::SetCollisionType(CollisionType type, bool cantPush)
{
	CollisionNodePath = collision->SetCollision(type, cantPush);
}
CollisionType GameObject::GetCollisionType()
{
	return collision->GetCollisionType();
}
void GameObject::ChangeCollisionType(CollisionType type, bool cantPush)
{
	CollisionNodePath = collision->ChangeCollision(type, cantPush);
}
bool GameObject::GetCollisionPushSetting()
{
	return collision->GetCantPushObjects();
}
#pragma endregion

#pragma region Trigger Component
//Add and change the object Trigger
void GameObject::AddTrigger()
{
	trigger = new Trigger();
}
void GameObject::AddTriggerInteractor()
{
	PT(CollisionNode) gameObject_Node;
	PT(CollisionBox) gameObject_SolidBox;

	LPoint3 min, max;
	Model.calc_tight_bounds(min, max);
	if (max == LPoint3(0, 0, 0)) {
		max = LPoint3(1, 1, 1);
	}
	
	std::ostringstream tag;
	tag << id;
	gameObject_SolidBox = new CollisionBox(min,max);
	gameObject_Node = new CollisionNode("TriggerInteract");
	gameObject_Node->set_tag("TriggerBox", tag.str());
	gameObject_Node->set_collide_mask(BitMask32::bit(3));
	gameObject_Node->add_solid(gameObject_SolidBox);
	TriggerNodePath = Model.attach_new_node(gameObject_Node);
	TriggerNodePath.reparent_to(Model);
	TriggerNodePath.show_bounds();
}
bool GameObject::HasTrigger()
{
	if (trigger != nullptr) {
		return true;
	}
	return false;
}
void GameObject::StoreTriggerActions(int enterID, int gameObjectID, int direction, int action, int actionID)
{
	trigger->AddNewAction(enterID, gameObjectID, direction, action, actionID);
}
void GameObject::StoreTriggerMoveTo(int enterID, int gameObjectID, LPoint3 newPos)
{
	trigger->MoveObjectAction(enterID, gameObjectID, newPos);
}
void GameObject::StoreTriggerScene(int enterID, std::string sceneName)
{
	trigger->SetChangeScene(enterID, sceneName);
}
void GameObject::ChangeTriggerAction(int id, int enterID, int selectedObjectID, int direction, int action, int actionID)
{
	trigger->GetTriggerAction(id).enteringObjectID = enterID;
	trigger->GetTriggerAction(id).connectedObjectID = selectedObjectID;
	trigger->GetTriggerAction(id).newAction = action;
	trigger->GetTriggerAction(id).newDirection = direction;
	trigger->GetTriggerAction(id).actionID = actionID;
}
void GameObject::ChangeTriggerMoveAction(int id, int enterID, int selectedObjectID, LPoint3 newPos)
{
	trigger->GetTriggerAction(id).enteringObjectID = enterID; 
	trigger->GetTriggerAction(id).connectedObjectID = selectedObjectID;
	trigger->GetTriggerAction(id).toPos = newPos;
}
void GameObject::ChangeTriggerScene(int id, int enterID, std::string sceneName)
{
	trigger->GetTriggerAction(id).enteringObjectID = enterID;
	trigger->GetTriggerAction(id).newScene = sceneName;
}
TriggerActions& GameObject::GetTriggerAction(int i)
{
	return trigger->GetTriggerAction(i);
}
void GameObject::LoadTriggerBox()
{
	Model = TriggerNodePath;
	CollisionNodePath = TriggerNodePath;

	Model.reparent_to(window->get_render());

	AttachEditorCollider();
}
#pragma endregion
