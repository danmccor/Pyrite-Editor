#include "Transform.h"

void Transform::AddAction(Action action, float speed, Direction direction, std::string key)
{
	TransformAction* movement = new TransformAction(action, key, speed, direction);
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
	return newPos;
}

LPoint3 Transform::Rotate(LPoint3 Rotation, MouseWatcher* mouseWatcher)
{
	LPoint3 newRot = Rotation;

	for (int i = 0; i < Actions.size(); i++) {
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
	return newRot;
}

TransformAction::TransformAction(Action action, std::string key, float speed, Direction direction)
{
	Key = key;
	Speed = speed;
	this->direction = direction;
	this->action = action;
}
