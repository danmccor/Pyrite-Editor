#pragma once
#include "collisionSolid.h"
#include "collisionRay.h"
#include "collisionSphere.h"
#include "collisionBox.h"
#include "collisionNode.h"
#include "boundingSphere.h"
#include "boundingBox.h"
#include "stringStream.h"
#include "Transform.h"
#include "Collision.h"

class GameObject
{
public:
	GameObject() {};
	GameObject(WindowFramework* window, std::string modelLocation);

	int id;

	bool operator==(const GameObject& rhs);
	bool operator!=(const GameObject& rhs);

	void LoadModel(PandaFramework* framework);

	void ChangePosition(float x, float y, float z);

	void ToggleHighlight();

	NodePath& GetColNodePath();
	NodePath& GetModelNodePath();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float scale);
	float GetPositionX();
	float GetPositionY();
	float GetPositionZ();
	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();
	float GetScale();

	void Run();

	void AddTransform();
	bool HasTransform();
	void AddTransformAction(Action action, std::string key, float speed, Direction direction);
	void ChangeTransformAction(int id, Action action, std::string key, float speed, Direction direction);
	TransformAction& GetTransformAction(int id);
	int GetNumberOfActions() { return transform->GetNumberOfActions(); };

	void AddCollision();
	bool HasCollision();
	void SetCollisionType(CollisionType type);
	CollisionType GetCollisionType();
	void ChangeCollisionType(CollisionType type);
	void RunCollision(GameObject gameObject);

	void AddTrigger();

	int testID = 1;
private:
	std::string ModelLocation;
	WindowFramework* window = nullptr;
	NodePath Model;
	NodePath CollisionNodePath;

	bool highlighted = false;

	NodePath gameObject_nodePath;
	Transform* transform = nullptr;
	Collision* collision = nullptr;
};




