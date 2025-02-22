#include "Trigger.h"

NodePath& Trigger::GetNodePath()
{
	return TriggerObject;
}

void Trigger::AddNewAction(int enterID, int objectID, int direction, int action, int actionID)
{
	TriggerActions triggerAction; 

	triggerAction.type = TriggerType::Change;
	triggerAction.enteringObjectID = enterID;
	triggerAction.connectedObjectID = objectID;
	triggerAction.newAction = action;
	triggerAction.newDirection = direction;
	triggerAction.actionID = actionID;

	TriggerActionsList.push_back(triggerAction);
}

void Trigger::MoveObjectAction(int enterID, int objectID, LPoint3 pos)
{
	TriggerActions triggerAction;
	triggerAction.type = TriggerType::MoveTo;
	triggerAction.enteringObjectID = enterID;
	triggerAction.connectedObjectID = objectID;
	triggerAction.toPos = pos;

	TriggerActionsList.push_back(triggerAction);
}

void Trigger::SetChangeScene(int enterID, int objectID, std::string newScene)
{
	TriggerActions triggerAction;
	triggerAction.type = TriggerType::Scene;
	triggerAction.enteringObjectID = enterID;
	triggerAction.connectedObjectID = objectID;
	triggerAction.newScene = newScene;

	TriggerActionsList.push_back(triggerAction);
}

TriggerActions& Trigger::GetTriggerAction(int i)
{
	if (i >= 0 && i < TriggerActionsList.size()) {
		return TriggerActionsList[i];
	}
}

