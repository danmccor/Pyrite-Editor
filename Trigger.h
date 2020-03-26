#pragma once
#include "Components.h"
enum class TriggerType {
	Box = 1, Sphere = 2
};
class TriggerActions {
public:
	int connectedObjectID;
	int transformActionID;
	int newDirection;
	int newAction;
	float newSpeed;
};

class Trigger : public Components
{
public: 
	Trigger() {};
	Trigger(NodePath Model, NodePath collider);
	Trigger(NodePath render, TriggerType trigger);
	NodePath& GetNodePath();
	void AddNewAction(int objectID, int actionId, int direction, int action, float speed);



private:
	NodePath TriggerObject;
	std::vector<TriggerActions> TriggerActionsList;
};

