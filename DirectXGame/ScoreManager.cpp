#include "ScoreManager.h"

void ScoreManager::Initialize() { 
	score_ = 0; 
}

void ScoreManager::AddScore(int value) { 
	score_ += value;
}

int ScoreManager::GetScore() const { 
	return score_; 
}
