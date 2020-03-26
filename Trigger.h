#pragma once
#include "Components.h"
enum class TriggerType {
	Box = 1, Sphere = 2
};


class Trigger : public Components
{
public: 
	Trigger() {};
	Trigger(NodePath Model, NodePath collider);
	Trigger(NodePath render, TriggerType trigger);
	NodePath& GetNodePath();
	int GetTestID();

private:
	NodePath TriggerObject;
	static int testId;
};

