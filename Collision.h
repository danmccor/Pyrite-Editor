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
	Sphere = 0, Box = 1, Polygon = 2
};

class Collision
{
public:
	Collision(NodePath* Model);
	NodePath SetCollision(CollisionType collision);
	bool CheckCollision();
	NodePath ChangeCollision(CollisionType collision);
	CollisionType* GetCollisionType();

	CollisionType* currentCollisionType = nullptr;
	NodePath* Model;
	NodePath gameObject_nodePath;
	PT(CollisionNode) gameObject_Node;
	PT(BoundingSphere) boundingSphere;
	PT(CollisionSphere) gameObject_SolidSphere;
	PT(CollisionBox) gameObject_SolidBox;


};



