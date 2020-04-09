#include "Trigger.h"

NodePath& Trigger::GetNodePath()
{
	return TriggerObject;
}

void Trigger::AddNewAction(int enterID, int objectID, int direction, int action, int actionID)
{
	TriggerActions triggerAction; 

	triggerAction.enteringObjectID = enterID;
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

