#pragma once
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "collisionNode.h"
#include "collisionSolid.h"
#include "collisionSphere.h"
#include "collisionBox.h"
#include "collisionPolygon.h"
#include "boundingVolume.h"
#include "boundingSphere.h"
#include "boundingBox.h"
#include "geometricBoundingVolume.h"

enum class CollisionType {
	 Box = 0, Sphere = 1
};

class Collision
{
public:
	Collision(NodePath* Model);
	NodePath SetCollision(CollisionType collision, bool cantPush);
	NodePath ChangeCollision(CollisionType collision, bool cantPush);
	CollisionType GetCollisionType();
	bool GetCantPushObjects();
	static int i;


private:
	CollisionType currentCollisionType;
	NodePath* Model;
	NodePath gameObject_nodePath;
	PT(CollisionNode) gameObject_Node;
	PT(CollisionBox) gameObject_SolidBox;
	PT(BoundingSphere) boundingSphere;
	PT(CollisionSphere) gameObject_SolidSphere;
	
	bool CantPushObjects = false;
	
};



