#include "Collision.h"

void Collision::SetCollision(CollisionType collision)
{
	std::string OutputString = "Collision Type: " + std::to_string((int)collision) + "\n";
	OutputDebugStringA(OutputString.c_str());
	static int i = 0;
	std::ostringstream tag;
	tag << i;
	PT(CollisionNode) gameObject_Node;
	switch (collision) {
	case CollisionType::Sphere:
	{
		OutputString = "Adding Collision Sphere \n";
		OutputDebugStringA(OutputString.c_str());
		PT(BoundingSphere) boundingSphere = DCAST(BoundingSphere, Model->get_bounds());
		PT(CollisionSphere) gameObject_Solid = new CollisionSphere(boundingSphere->get_center(), boundingSphere->get_radius());
		gameObject_Node = new CollisionNode("Sphere");
		gameObject_Node->add_solid(gameObject_Solid);
		break;
	}
	case CollisionType::Box:
	{
		LPoint3 min, max;
		Model->calc_tight_bounds(min, max);
		PT(CollisionBox) gameObject_Solid = new CollisionBox(min, max);
		gameObject_Node = new CollisionNode("Box");
		gameObject_Node->add_solid(gameObject_Solid);
		break;
	}
	case CollisionType::Polygon:
		Model->set_collide_mask(BitMask32::bit(1));
		Model->set_tag("Object", tag.str());
		break;
	}
	if (collision != CollisionType::Polygon) {
		gameObject_Node->set_collide_mask(BitMask32::bit(1));
		gameObject_Node->set_tag("Object", tag.str());
		gameObject_nodePath = Model->attach_new_node(gameObject_Node);
		gameObject_nodePath.show();
	}
	i++;
}

bool Collision::CheckCollision()
{

	return false;
}
