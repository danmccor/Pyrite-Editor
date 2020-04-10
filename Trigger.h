#pragma once
#include "Components.h"

enum class TriggerType {
	Change = 0, MoveTo = 1
};

enum class TriggerShape {
	Box = 0, Sphere = 1
};
class TriggerActions {
public:
	TriggerType type;
	int enteringObjectID;
	int connectedObjectID;
	int newDirection;
	int newAction;
	int actionID;
	LPoint3 toPos;
};

class Trigger : public Components
{
public: 
	Trigger() {};
	NodePath& GetNodePath();
	void AddNewAction(int enterID, int objectID, int direction, int action, int actionID);

	void MoveObjectAction(int enterID, int objectID, LPoint3 pos);

	int GetNumberOfTriggerActions() { return TriggerActionsList.size(); };
	TriggerActions& GetTriggerAction(int i);

private:
	NodePath TriggerObject;
	std::vector<TriggerActions> TriggerActionsList;
};

