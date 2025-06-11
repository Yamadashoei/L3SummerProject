#pragma once
#include "KamataEngine.h"
#include "kMath.h"

class Collision {
public:
	Collision(const KamataEngine::Vector3& position = {0.0f, 0.0f, 0.0f}, float radius = 1.0f) : position_(position), radius_(radius) {}

	void SetPosition(const KamataEngine::Vector3& position) { position_ = position; }
	void SetRadius(float radius) { radius_ = radius; }

	const KamataEngine::Vector3& GetPosition() const { return position_; }
	float GetRadius() const { return radius_; }

	bool CheckCollision(const Collision& other) const;

private:
	KamataEngine::Vector3 position_;
	float radius_;
};
