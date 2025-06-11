#include "kMath.h"
#include <cassert>
#include <cmath>
#include <numbers>

// Vector3の足し算の演算子オーバーロード
KamataEngine::Vector3& operator+=(KamataEngine::Vector3& lhv, const KamataEngine::Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

// Vector3のスカラー掛け算の演算子オーバーロード
KamataEngine::Vector3& operator*=(KamataEngine::Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

// Vector3のスカラー掛け算
const KamataEngine::Vector3 operator*(const KamataEngine::Vector3& v, float s) {
	KamataEngine::Vector3 temp(v);
	return temp *= s;
}

// Vector3同士の足し算
const KamataEngine::Vector3 operator+(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2) {
	KamataEngine::Vector3 temp(v1);
	return temp += v2;
}

// Vector3同士の引き算
const KamataEngine::Vector3 operator-(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2) { return KamataEngine::Vector3{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }

// 2つの行列を掛け算する関数
KamataEngine::Matrix4x4 Multiply(KamataEngine::Matrix4x4 matrix1, KamataEngine::Matrix4x4 matrix2) {
	KamataEngine::Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}
	return result;
}

// 拡大縮小行列を作成する関数
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale) {
	KamataEngine::Matrix4x4 ans = {0};
	ans.m[0][0] = scale.x;
	ans.m[1][1] = scale.y;
	ans.m[2][2] = scale.z;
	ans.m[3][3] = 1;
	return ans;
}

// 平行移動行列を作成する関数
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate) {
	KamataEngine::Matrix4x4 ans = {0};
	ans.m[0][0] = 1;
	ans.m[1][1] = 1;
	ans.m[2][2] = 1;
	ans.m[3][0] = translate.x;
	ans.m[3][1] = translate.y;
	ans.m[3][2] = translate.z;
	ans.m[3][3] = 1;
	return ans;
}

// X軸周りの回転行列を作成する関数
KamataEngine::Matrix4x4 MakeRotateXMatrix(float radian) {
	KamataEngine::Matrix4x4 ans = {0};
	ans.m[0][0] = 1;
	ans.m[1][1] = cosf(radian);
	ans.m[1][2] = sinf(radian);
	ans.m[2][1] = -sinf(radian);
	ans.m[2][2] = cosf(radian);
	ans.m[3][3] = 1;
	return ans;
}

// Y軸周りの回転行列を作成する関数
KamataEngine::Matrix4x4 MakeRotateYMatrix(float radian) {
	KamataEngine::Matrix4x4 ans = {0};
	ans.m[0][0] = cosf(radian);
	ans.m[0][2] = -sinf(radian);
	ans.m[1][1] = 1;
	ans.m[2][0] = sinf(radian);
	ans.m[2][2] = cosf(radian);
	ans.m[3][3] = 1;
	return ans;
}

// Z軸周りの回転行列を作成する関数
KamataEngine::Matrix4x4 MakeRotateZMatrix(float radian) {
	KamataEngine::Matrix4x4 ans = {0};
	ans.m[0][0] = cosf(radian);
	ans.m[0][1] = sinf(radian);
	ans.m[1][0] = -sinf(radian);
	ans.m[1][1] = cosf(radian);
	ans.m[2][2] = 1;
	ans.m[3][3] = 1;
	return ans;
}

// 3次元アフィン変換行列を作成する関数
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translation) {
	KamataEngine::Matrix4x4 ans;
	ans = Multiply(Multiply(MakeScaleMatrix(scale), Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z))), MakeTranslateMatrix(translation));
	return ans;
}

KamataEngine::Matrix4x4 PlayerAffineMatrix(const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate) {
	KamataEngine::Matrix4x4 ansMat = Multiply(MakeRotateYMatrix(rotate.y), MakeTranslateMatrix(translate));
	return ansMat;
}


// EaseInOut関数
float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

// 線形補間関数
float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

// ベクトルの線形補間関数
KamataEngine::Vector3 Lerp(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2, float t) { return KamataEngine::Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }

// ベクトルに行列を掛けて変換する関数
KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix) {
	KamataEngine::Vector3 ans;
	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

// 法線ベクトルの変換
KamataEngine::Vector3 TransformNormal(const KamataEngine::Vector3& v, const KamataEngine::Matrix4x4& m) {
	KamataEngine::Vector3 result{v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}
