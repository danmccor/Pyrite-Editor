#pragma once
#include "Components.h"
enum class TriggerType {
	Box = 1, Sphere = 2
};
class TriggerActions {
public:
	int enteringObjectID;
	int connectedObjectID;
	int newDirection;
	int newAction;
	int actionID;
};

class Trigger : public Components
{
public: 
	Trigger() {};
	NodePath& GetNodePath();
	void AddNewAction(int enterID, int objectID, int direction, int action, int actionID);
	int GetNumberOfTriggerActions() { return TriggerActionsList.size(); };
	TriggerActions& GetTriggerAction(int i);

private:
	NodePath TriggerObject;
	std::vector<TriggerActions> TriggerActionsList;
};

