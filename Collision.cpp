#include "Collision.h"

Collision::Collision(NodePath* Model)
{
	 this->Model = Model;
	 boundingSphere = DCAST(BoundingSphere, Model->get_bounds());
	 gameObject_SolidSphere = new CollisionSphere(boundingSphere->get_center(), boundingSphere->get_radius());

	 LPoint3 min, max;
	 Model->calc_tight_bounds(min, max);
	 gameObject_SolidBox = new CollisionBox(min, max);

	 gameObject_Node = new CollisionNode("CollisionType");


}

NodePath Collision::SetCollision(CollisionType collision)
{
	LPoint3 tmp = Model->get_pos();
	Model->set_pos(0, 0, 0);
	static int i = 0;
	std::ostringstream tag;
	tag << i; 
	switch (collision) {
	case CollisionType::Sphere:
	{
		/*boundingSphere = DCAST(BoundingSphere, Model->get_bounds());
		gameObject_SolidSphere = new CollisionSphere(boundingSphere->get_center(), boundingSphere->get_radius());
		gameObject_Node = new CollisionNode("Sphere");*/
		currentCollisionType = CollisionType::Sphere;
		gameObject_Node->add_solid(gameObject_SolidSphere);
		break;
	}
	case CollisionType::Box:
	{
		/*LPoint3 min, max;
		Model->calc_tight_bounds(min, max);
		gameObject_SolidBox = new CollisionBox(min, max);
		gameObject_Node = new CollisionNode("Box");*/
		currentCollisionType = CollisionType::Box;
		gameObject_Node->add_solid(gameObject_SolidBox);
		break;
	}
	case CollisionType::Polygon:
		Model->set_collide_mask(BitMask32::bit(2));
		Model->set_tag("Object", tag.str());
		currentCollisionType = CollisionType::Polygon;
		break;
	}
	if (collision != CollisionType::Polygon) {
		gameObject_Node->set_collide_mask(BitMask32::bit(2));
		gameObject_Node->set_tag("Object", tag.str());
		gameObject_nodePath = Model->attach_new_node(gameObject_Node);
		//gameObject_nodePath.set_pos(Model->get_pos());
		gameObject_nodePath.show();
	}
	
	gameObject_nodePath.reparent_to(*Model);
	Model->set_pos(tmp);
	i++;
	return gameObject_nodePath;

	
}


NodePath Collision::ChangeCollision(CollisionType collision)
{
	gameObject_Node->clear_solids();

	switch (collision) {
	case CollisionType::Box:
		currentCollisionType = CollisionType::Box;
		gameObject_Node->add_solid(gameObject_SolidBox);
		break;

	case CollisionType::Sphere:
		currentCollisionType = CollisionType::Sphere;
		gameObject_Node->add_solid(gameObject_SolidSphere);
		break;
	case CollisionType::Polygon:
		currentCollisionType = CollisionType::Polygon;
		break;
	}
	return gameObject_nodePath;
}


CollisionType Collision::GetCollisionType()
{
	return currentCollisionType;
}
