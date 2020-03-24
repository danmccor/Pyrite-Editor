#pragma once
#include "Components.h"
enum class TriggerType {
	Box = 1, Sphere = 2
};


class Trigger : public Components
{
public: 
	Trigger();
	Trigger(NodePath render, TriggerType trigger);
	NodePath GetNodePath();

private:
	NodePath TriggerObject;
};

