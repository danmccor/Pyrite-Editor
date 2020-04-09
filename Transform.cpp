#include "Transform.h"

void Transform::AddAction(Action action, float speed, Direction direction, std::string key)
{
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
	if (key >= 0 && key < Actions.size()) {
		return *Actions[key];
	}
}

LPoint3 Transform::Move(LPoint3 Position, LPoint3 Forward, LPoint3 Up, MouseWatcher* mouseWatcher)
{
	LPoint3 newPos = Position;
	LPoint3 Right = cross(Forward, Up);
	for (int i = 0; i < Actions.size(); i++) {
		if (Actions[i]->type == TransformType::Add) {
			if (Actions[i]->action == Action::Move) {
				char key;
				strcpy(&key, &Actions[i]->Key[0]);
				if (mouseWatcher->is_button_down(KeyboardButton::ascii_key(key)) || Actions[i]->Key == "") {
					if (Actions[i]->direction == Direction::Forward) {

						newPos += (Forward * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Backward) {
						newPos -= (Forward * Actions[i]->Speed) * globalClock.get_dt();
					}
					if (Actions[i]->direction == Direction::Right) {
						newPos += (Right * Actions[i]->Speed) * globalClock.get_dt();
					}

					if (Actions[i]->direction == Direction::Left) {
						newPos += (-Right * Actions[i]->Speed) * globalClock.get_dt();

					}
					if (Actions[i]->direction == Direction::Up) {
						newPos += (Up * Actions[i]->Speed) * globalClock.get_dt();
					}
				}
				if (Actions[i]->direction == Direction::Down) {
					newPos += (-Up * Actions[i]->Speed) * globalClock.get_dt();
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

						newRot.set_x(newRot.get_x() - (Actions[i]->Speed * globalClock.get_dt()));

					}
					if (Actions[i]->direction == Direction::Right) {
						newRot.set_x(newRot.get_x() + (Actions[i]->Speed * globalClock.get_dt()));
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

