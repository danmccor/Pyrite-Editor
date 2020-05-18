#include "Collision.h"

int Collision::i = 0;

Collision::Collision(NodePath* Model)
{
	//Store Model
	 this->Model = Model;
	 //Create bounding sphere
	 boundingSphere = DCAST(BoundingSphere, Model->get_bounds());
	 //Get centre of sphere and radius
	 gameObject_SolidSphere = new CollisionSphere(boundingSphere->get_center(), boundingSphere->get_radius());
	 //Create min, max vectors
	 LPoint3 min, max;
	 //Get the bounds with min and max
	 Model->calc_tight_bounds(min, max);
	 //Create solid box using min and max
	 gameObject_SolidBox = new CollisionBox(min, max);
	 //Create node for collision
	 gameObject_Node = new CollisionNode("CollisionType");
}

NodePath Collision::SetCollision(CollisionType collision, bool cantPush)
{
	//Set cant push objects 
	CantPushObjects = cantPush;
	//Get position of model
	LPoint3 tmp = Model->get_pos();
	//Set position to 0
	Model->set_pos(0, 0, 0);
	//Set tag to static int
	std::ostringstream tag;
	tag << i; 
	//Switch collision type (Sphere or Box)
	switch (collision) {
	case CollisionType::Sphere:
	{
		//Add the sphere solid
		currentCollisionType = CollisionType::Sphere;
		gameObject_Node->add_solid(gameObject_SolidSphere);
		break;
	}
	case CollisionType::Box:
	{
		//Add the box solid
		currentCollisionType = CollisionType::Box;
		gameObject_Node->add_solid(gameObject_SolidBox);
		break;
	}
	}
		//Set mask to 2
		gameObject_Node->set_collide_mask(BitMask32::bit(2));
		//Set the tag to object and the unique tag
		gameObject_Node->set_tag("Object", tag.str());
		gameObject_nodePath = Model->attach_new_node(gameObject_Node);
		//gameObject_nodePath.set_pos(Model->get_pos());
		//Show the collider
		gameObject_nodePath.show();
	
	gameObject_nodePath.reparent_to(*Model);
	//Set model back to original pos
	Model->set_pos(tmp);
	i++;
	return gameObject_nodePath;
}


NodePath Collision::ChangeCollision(CollisionType collision, bool cantPush)
{
	//Set bool and clear the solids from the game object
	CantPushObjects = cantPush;
	gameObject_Node->clear_solids();

	//Set solid depending on collision type
	switch (collision) {
	case CollisionType::Box:
		currentCollisionType = CollisionType::Box;
		gameObject_Node->add_solid(gameObject_SolidBox);
		break;

	case CollisionType::Sphere:
		currentCollisionType = CollisionType::Sphere;
		gameObject_Node->add_solid(gameObject_SolidSphere);
		break;
	}
	return gameObject_nodePath;
}


CollisionType Collision::GetCollisionType()
{
	return currentCollisionType;
}

bool Collision::GetCantPushObjects()
{
	return CantPushObjects;
}
