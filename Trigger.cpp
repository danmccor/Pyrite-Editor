#include "Trigger.h"
Trigger::Trigger(NodePath Model, NodePath collider)
{
	//TriggerObject = collider.copy_to(TriggerObject);
	PT(CollisionNode) gameObject_Node;
	PT(CollisionBox) gameObject_SolidBox;

	LPoint3 min, max;
	Model.calc_tight_bounds(min, max);
	gameObject_SolidBox = new CollisionBox(min, max);
	gameObject_Node = new CollisionNode("CollisionType");
	gameObject_Node->set_collide_mask(BitMask32::bit(3));
	gameObject_Node->add_solid(gameObject_SolidBox);
	TriggerObject = Model.attach_new_node(gameObject_Node);


	//std::string Output = "Gets Here \n";
	//OutputDebugStringA(Output.c_str());
	////TriggerObject.set_collide_mask
	////TriggerObject.node()->set_into_collide_mask(BitMask32::bit(3));
	//Output = "Crashes Here? \n";
	//OutputDebugStringA(Output.c_str());

	TriggerObject.reparent_to(Model);
	TriggerObject.show();
}

Trigger::Trigger(NodePath render, TriggerType trigger)
{
	switch (trigger) {
	case TriggerType::Sphere:
	{
		PT(CollisionSphere) sphere = new CollisionSphere(0, 0, 0, 1);
		PT(CollisionNode) triggerCollider = new CollisionNode("TriggerCollider");
		triggerCollider->add_solid(sphere);
		triggerCollider->set_into_collide_mask(BitMask32::bit(3));
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
		triggerCollider->set_collide_mask(BitMask32::bit(3));
		TriggerObject = render.attach_new_node(triggerCollider);
		//TriggerObject.set_collide_mask(BitMask32::bit(2));
		TriggerObject.show();
		break;
	}
	}
}

NodePath& Trigger::GetNodePath()
{
	return TriggerObject;
}

void Trigger::AddNewAction(int objectID, int direction, int action, int actionID)
{
	TriggerActions triggerAction; 

	triggerAction.connectedObjectID = objectID;
	triggerAction.newAction = action;
	triggerAction.newDirection = direction;
	triggerAction.actionID = actionID;

	TriggerActionsList.push_back(triggerAction);
}

TriggerActions& Trigger::GetTriggerAction(int i)
{
	if (i >= 0 && i < TriggerActionsList.size()) {
		return TriggerActionsList[i];
	}
}

