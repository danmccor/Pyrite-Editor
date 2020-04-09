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
//Public functions
public:
	//Object constructors
	GameObject() {};
	GameObject(WindowFramework* window, std::string modelLocation);
	GameObject(WindowFramework* window, TriggerType triggerType);
	//Compare object
	bool operator==(const GameObject& rhs);
	bool operator!=(const GameObject& rhs);

	//Load the object model
	virtual void LoadModel(PandaFramework* framework);

	//Toggle the heighlight around object
	void ToggleHighlight();

	//Get object nodepaths
	NodePath& GetTriggerNodePath();
	NodePath& GetColNodePath();
	NodePath& GetModelNodePath();

	//Set the objects name
	void SetObjectName(std::string name);

	//Get the object name and file location
	std::string GetObjectName();
	std::string GetFileLocation();

	//Manipulate objects positions
	void ChangePosition(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	float GetPositionX();
	float GetPositionY();
	float GetPositionZ();
	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();
	float GetScale();
	float GetScaleX();
	float GetScaleY();
	float GetScaleZ();

	//Run components
	void Run(MouseWatcher* mouseWatcher);

	//Transform Component
	void AddTransform();
	bool HasTransform();
	void AddTransformAction(Action action, std::string key, float speed, Direction direction);
	void ChangeTransformAction(int id, Action action, std::string key, float speed, Direction direction);
	void AddTranformFollowAction(TransformAxis axis, int selectedObject, std::string key, float speed);
	void ChangeTransformFollowAction(int id, TransformAxis axis, int selectedObject, std::string key, float speed);
	TransformAction& GetTransformAction(int id);
	int GetNumberOfActions() { return transform->GetNumberOfActions(); };
	void TransformFollowObject(int id, GameObject* object, MouseWatcher* mouseWatcher);

	//Collision component
	void AddCollision();
	bool HasCollision();
	void SetCollisionType(CollisionType type, bool cantPush);
	CollisionType GetCollisionType();
	void ChangeCollisionType(CollisionType type, bool cantPush);
	bool GetCollisionPushSetting();

	//Trigger component
	void AddTrigger();
	void AddTriggerInteractor();
	bool HasTrigger();
	void StoreTriggerActions(int enterID, int gameObjectID, int direction, int action, int actionID);
	void ChangeTriggerAction(int id, int enterID, int selectedObjectID, int direction, int action, int actionID);
	int GetNumberOfTriggerActions() { return trigger->GetNumberOfTriggerActions(); };
	TriggerActions& GetTriggerAction(int i);

	//Camera objects
	bool IsCamera() { return isCamera; };
	virtual void ActivateCamera();

	//Delete object
	void Delete();
	void ResetTag();

//Public variables
public: 
	static int objectCount;
	int id;

//Protected functions
protected:
	void SetFileLocation(std::string location);
	void LoadTriggerBox();
	void AttachEditorCollider();

//Protected variables
protected:
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

	bool isCamera = false;

	std::string ObjectName = "";
};




