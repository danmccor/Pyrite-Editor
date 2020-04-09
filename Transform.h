#pragma once
#include "Components.h"
#include <vector>
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "mouseWatcher.h"

extern ClockObject globalClock;

enum class Direction
{
	Forward = 0, Backward = 1, Left = 2, Right = 3, Up = 4, Down = 5
};

enum class Action
{
	Move = 0, Rotate = 1
};

class TransformAction {
public:
	TransformAction(Action action, std::string key, float speed, Direction direction);
	std::string Key = "";
	float Speed;
	Direction direction;
	Action action;
};

class Transform : public Components
{
public: 
	void AddAction(Action action, float speed, Direction direction, std::string key = "");
	TransformAction& GetAction(int key);
	int GetNumberOfActions() { return Actions.size(); };
	//Position is the Current Objects position, Forward is the direction it is currently facing
	LPoint3 Move(LPoint3 Position, LPoint3 Forward, LPoint3 Up, MouseWatcher* mouseWatcher);
	LPoint3 Rotate(LPoint3 Rotation, MouseWatcher* mouseWatcher);

private:
	std::vector<TransformAction*> Actions;
};


