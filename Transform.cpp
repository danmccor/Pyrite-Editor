#include "Transform.h"

void Transform::AddAction(Action action, float speed, Direction direction, std::string key)
{
	//Set the transform Action
	TransformAction* movement = new TransformAction();
	movement->type = TransformType::Add;
	movement->action = action;
	movement->Speed = speed;
	movement->Key = key;
	movement->direction = direction;
	Actions.push_back(movement);
}

void Transform::AddFollowAction(TransformAxis axis, int selectedObjectID, std::string key, float speed)
{
	//Set the transform action
	TransformAction* movement = new TransformAction();
	movement->type = TransformType::Follow;
	movement->axis = axis;
	movement->ConnectedObject = selectedObjectID;
	movement->Key = key;
	movement->Speed = speed;
	Actions.push_back(movement);
}

TransformAction& Transform::GetAction(int key)
{
	//Get the transform action
	if (key >= 0 && key < Actions.size()) {
		return *Actions[key];
	}
}

LPoint3 Transform::Move(LPoint3 Position, LPoint3 Forward, LPoint3 Up, MouseWatcher* mouseWatcher)
{
	//Get the objects position
	LPoint3 newPos = Position;
	//Get the objects right vector
	LPoint3 Right = cross(Forward, Up);
	for (int i = 0; i < Actions.size(); i++) {
		//If the transform type is add
		if (Actions[i]->type == TransformType::Add) {
			if (Actions[i]->action == Action::Move) {
				char key;
				strcpy(&key, &Actions[i]->Key[0]);
				//If key is not set or if the button is being held down
				if (mouseWatcher->is_button_down(KeyboardButton::ascii_key(key)) || Actions[i]->Key == "") {
					if (Actions[i]->direction == Direction::Forward) {
						//Move forward
						newPos += (Forward * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Backward) {
						//Move Backward
						newPos -= (Forward * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Right) {
						//Move Right
						newPos += (Right * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Left) {
						//move Left
						newPos += (-Right * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Up) {
						//Move Up
						newPos += (Up * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Down) {
						//Move Down
						newPos += (-Up * Actions[i]->Speed) * globalClock.get_dt();
					}
				}
			}
		}
	}
	return newPos;
}

LPoint3 Transform::Rotate(LPoint3 Rotation, MouseWatcher* mouseWatcher)
{
	LPoint3 newRot = Rotation;

	for (int i = 0; i < Actions.size(); i++) {
		if (Actions[i]->type == TransformType::Add) {
			if (Actions[i]->action == Action::Rotate) {
				char key;
				strcpy(&key, &Actions[i]->Key[0]);
				if (mouseWatcher->is_button_down(KeyboardButton::ascii_key(key)) || Actions[i]->Key == "") {
					if (Actions[i]->direction == Direction::Left) {
						//Rotate Left
						newRot.set_x(newRot.get_x() - (Actions[i]->Speed * globalClock.get_dt()));
					}
					if (Actions[i]->direction == Direction::Right) {
						//Rotate Right
						newRot.set_x(newRot.get_x() + (Actions[i]->Speed * globalClock.get_dt()));
					}
					if (Actions[i]->direction == Direction::Down) {
						//Rotate Left
						newRot.set_y(newRot.get_y() - (Actions[i]->Speed * globalClock.get_dt()));
					}
					if (Actions[i]->direction == Direction::Up) {
						//Rotate Right
						newRot.set_y(newRot.get_y() + (Actions[i]->Speed * globalClock.get_dt()));
					}
				}
			}
		}
	}
	return newRot;
}

LPoint3 Transform::FollowMove(int id, LPoint3 Position, LPoint3 TargetPos, MouseWatcher* mouseWatcher)
{
	LPoint3 newPos = Position;
			LPoint3 direction = TargetPos - newPos;
			switch (Actions[id]->axis) {
			case TransformAxis::X:
				if (direction.get_x() > 0) {
					newPos += (LPoint3(1,0,0) * Actions[id]->Speed) * globalClock.get_dt();
				}
				if (direction.get_x() < 0) {
					newPos -= (LPoint3(1, 0, 0) * Actions[id]->Speed) * globalClock.get_dt();
				}
				break;
			case TransformAxis::Y:
				if (direction.get_y() > 0) {
					newPos += (LPoint3(0, 1, 0) * Actions[id]->Speed) * globalClock.get_dt();
				}
				if (direction.get_y() < 0) {
					newPos -= (LPoint3(0, 1, 0) * Actions[id]->Speed) * globalClock.get_dt();
				}
				break;
			case TransformAxis::Z:
				if (direction.get_z() > 0) {
					newPos += (LPoint3(0, 0, 1) * Actions[id]->Speed) * globalClock.get_dt();
				}
				if (direction.get_z() < 0) {
					newPos -= (LPoint3(0, 0, 1) * Actions[id]->Speed) * globalClock.get_dt();
				}
				break;
			
	}
	return newPos;
}

