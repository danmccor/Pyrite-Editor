#pragma once
#include "Components.h"
enum class TriggerType {
	Box = 1, Sphere = 2
};
class TriggerActions {
public:
	int connectedObjectID;
	int newDirection;
	int newAction;
	int actionID;
};

class Trigger : public Components
{
public: 
	Trigger() {};
	Trigger(NodePath Model, NodePath collider);
	Trigger(NodePath render, TriggerType trigger);
	NodePath& GetNodePath();
	void AddNewAction(int objectID, int direction, int action, int actionID);
	int GetNumberOfTriggerActions() { return TriggerActionsList.size(); };
	TriggerActions& GetTriggerAction(int i);

private:
	NodePath TriggerObject;
	std::vector<TriggerActions> TriggerActionsList;
};

