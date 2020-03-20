#include "GameObject.h"
#include <ostream>

GameObject::GameObject(WindowFramework* window, std::string modelLocation)
{
	this->window = window;
	this->ModelLocation = modelLocation;
}

void GameObject::LoadModel(PandaFramework* framework)
{
	Model = window->load_model(framework->get_models(), ModelLocation);
	Model.reparent_to(window->get_render());
	PT(BoundingSphere) boundingSphere = DCAST(BoundingSphere, Model.get_bounds());
	PT(CollisionSphere) gameObject_Solid = new CollisionSphere(boundingSphere->get_center(), boundingSphere->get_radius());

	static int i = 0;
	std::ostringstream tag;
	tag << i;
	PT(CollisionNode) gameObject_Node = new CollisionNode("Sphere");
	gameObject_Node->add_solid(gameObject_Solid);
	gameObject_Node->set_into_collide_mask(BitMask32::bit(1));
	gameObject_Node->set_tag("Object", tag.str());
	gameObject_nodePath = Model.attach_new_node(gameObject_Node);
	i++;
	//gameObject_nodePath.show();

}

void GameObject::SetPosition(float x, float y, float z)
{
	Model.set_pos(x, y, z);
}

void GameObject::SetRotation(float x, float y, float z)
{
	Model.set_hpr(x, y, z);
}

void GameObject::SetScale(float scale)
{
	Model.set_scale(scale, scale, scale);
}

void GameObject::ChangePosition(float x, float y, float z)
{
	Model.set_pos(Model.get_pos().get_x() + x, Model.get_pos().get_y() + y, Model.get_pos().get_z() + z);
}

void GameObject::ToggleHighlight()
{
	if (highlighted) {
		gameObject_nodePath.hide();
		highlighted = false;
	}
	else {
		gameObject_nodePath.show();
		highlighted = true;
	}
}

NodePath GameObject::getColNodePath()
{
	return gameObject_nodePath;
}

NodePath GameObject::GetModelNodePath()
{
	return Model;
}

float GameObject::GetPositionX()
{
	// TODO: insert return statement here
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

float GameObject::GetScale()
{
	return Model.get_scale().get_x();
}

void GameObject::Run()
{
	if (transform != nullptr) {
		Model.set_pos(transform->Move(Model.get_pos(), Model.get_net_transform()->get_mat().get_row3(1), Model.get_net_transform()->get_mat().get_row3(2)));
		Model.set_hpr(transform->Rotate(Model.get_hpr()));
	}
}

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

void GameObject::AddCollision()
{
}

void GameObject::AddTrigger()
{
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

TransformAction& GameObject::GetTransformAction(int id)
{
	 return transform->GetAction(id);
	// TODO: insert return statement here
}

