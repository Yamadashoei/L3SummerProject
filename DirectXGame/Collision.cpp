#include "Collision.h"
#include "kMath.h"

bool Collision::CheckCollision(const Collision& other) const {
	float distance = Vector3Length(position_ - other.position_);
	return distance <= (radius_ + other.radius_);
}
