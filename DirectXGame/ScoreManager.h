#pragma once
class ScoreManager {
public:
	void Initialize();
	void AddScore(int value);
	int GetScore() const;

private:
	int score_ = 0;
};
