#include "BaseEnemy.h"

void BaseEnemy::TakeDamage(int damage) {
	hp_ -= damage;
	if (hp_ < 0) {
		hp_ = 0;
	}
}
