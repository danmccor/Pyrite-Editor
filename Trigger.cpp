#include "Trigger.h"

Trigger::Trigger()
{

}

Trigger::Trigger(NodePath render, TriggerType trigger)
{
	switch (trigger) {
	case TriggerType::Sphere:
	{
		PT(CollisionSphere) sphere = new CollisionSphere(0, 0, 0, 1);
		PT(CollisionNode) triggerCollider = new CollisionNode("TriggerCollider");
		triggerCollider->add_solid(sphere);
		triggerCollider->set_into_collide_mask(BitMask32::bit(2));
		TriggerObject = render.attach_new_node(triggerCollider);
		//TriggerObject.set_collide_mask(BitMask32::bit(2));
		TriggerObject.show();
		break;
	}
	case TriggerType::Box:
	{
		PT(CollisionBox) box = new CollisionBox(LPoint3(0, 0, 0), 1, 1, 1);
		PT(CollisionNode) triggerCollider = new CollisionNode("TriggerCollider");
		triggerCollider->add_solid(box);
		triggerCollider->set_collide_mask(0);
		triggerCollider->set_into_collide_mask(BitMask32::bit(2));
		TriggerObject = render.attach_new_node(triggerCollider);
		//TriggerObject.set_collide_mask(BitMask32::bit(2));
		TriggerObject.show();
		break;
	}
	}
}

NodePath Trigger::GetNodePath()
{
	return TriggerObject;
}
