#pragma once

enum class CollisionType {
	Sphere, Box, Capsule, Polygon
};

class Collision
{
public:
	Collision() {};
	void SetCollision(CollisionType collision);


	CollisionType* currentCollisionType = nullptr;

};



