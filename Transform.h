#pragma once
#include "Components.h"
#include <vector>
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "mouseWatcher.h"

extern ClockObject globalClock;

enum class TransformType {
	Add = 0, Follow = 1
};

enum class Direction
{
	Forward = 0, Backward = 1, Left = 2, Right = 3, Up = 4, Down = 5
};

enum class Action
{
	Move = 0, Rotate = 1
};

enum class TransformAxis {
	X = 0, Y = 1, Z = 2
};

class TransformAction {
public:
	TransformType type = TransformType::Add;
	std::string Key = "";
	float Speed;


	TransformAxis axis;
	int ConnectedObject;

	Direction direction;
	Action action;
};

class Transform : public Components
{
public: 
	void AddAction(Action action, float speed, Direction direction, std::string key = "");
	void AddFollowAction(TransformAxis axis, int selectedObjectID, std::string key, float speed);
	TransformAction& GetAction(int key);
	int GetNumberOfActions() { return Actions.size(); };
	//Position is the Current Objects position, Forward is the direction it is currently facing
	LPoint3 Move(LPoint3 Position, LPoint3 Forward, LPoint3 Up, MouseWatcher* mouseWatcher);
	LPoint3 Rotate(LPoint3 Rotation, MouseWatcher* mouseWatcher);
	LPoint3 FollowMove(int id, LPoint3 Position, LPoint3 TargetPos, MouseWatcher* mouseWatcher);

private:
	std::vector<TransformAction*> Actions;
};


