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
#include "Trigger.h"

class GameObject
{
public:
	GameObject() {};
	GameObject(WindowFramework* window, std::string modelLocation);
	GameObject(WindowFramework* window, TriggerType triggerType);

	static int objectCount;

	int id;

	bool operator==(const GameObject& rhs);
	bool operator!=(const GameObject& rhs);

	void LoadModel(PandaFramework* framework);
	void LoadTriggerBox();
	void AttachEditorCollider();

	void ChangePosition(float x, float y, float z);

	void ToggleHighlight();

	NodePath& GetTriggerNodePath();
	NodePath& GetColNodePath();
	NodePath& GetModelNodePath();

	void SetObjectName(std::string name);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float scale);
	std::string GetObjectName();
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

	void AddTrigger();
	void AddTriggerInteractor();
	bool HasTrigger();
	void StoreTriggerActions(int gameObjectID, int direction, int action, int actionID);
	void ChangeTriggerAction(int id, int selectedObjectID, int direction, int action, int actionID);
	int GetNumberOfTriggerActions() { return trigger->GetNumberOfTriggerActions(); };
	TriggerActions& GetTriggerAction(int i);
	void RunTrigger();

private:
	std::string ModelLocation;
	WindowFramework* window = nullptr;
	NodePath Model;
	NodePath CollisionNodePath;
	NodePath TriggerNodePath;
	PandaNode* triggerBox;
	bool highlighted = false;

	NodePath gameObject_nodePath;
	Transform* transform = nullptr;
	Collision* collision = nullptr;
	Trigger* trigger = nullptr;

	std::string ObjectName = "";
};




