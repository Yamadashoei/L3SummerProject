#pragma once
#include <KamataEngine.h>
#include <math.h>
#include <math\Matrix4x4.h>
#include <math\Vector3.h>

// 2つの行列を掛け算する関数（行列同士の積）
KamataEngine::Matrix4x4 Multiply(KamataEngine::Matrix4x4 matrix1, KamataEngine::Matrix4x4 matrix2);

// 拡大縮小行列を作成する関数
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);

// 平行移動行列を作成する関数
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);

// X軸周りの回転行列を作成する関数
KamataEngine::Matrix4x4 MakeRotateXMatrix(float radian);

// Y軸周りの回転行列を作成する関数
KamataEngine::Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸周りの回転行列を作成する関数
KamataEngine::Matrix4x4 MakeRotateZMatrix(float radian);

// 3次元アフィン変換行列を作成する関数
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translation);

// プレイヤー専用のアフィン変換行列を作成する関数
KamataEngine::Matrix4x4 PlayerAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translation);

// EaseInOut補間関数
float EaseInOut(float x1, float x2, float t);

// 線形補間関数（Lerp）
float Lerp(float x1, float x2, float t);

// ベクトルの線形補間関数
KamataEngine::Vector3 Lerp(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2, float t);

// Vector3同士の足し算の演算子オーバーロード
KamataEngine::Vector3& operator+=(KamataEngine::Vector3& lhs, const KamataEngine::Vector3& rhv);

// Vector3同士の足し算
const KamataEngine::Vector3 operator+(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

// Vector3のスカラー掛け算の演算子オーバーロード
KamataEngine::Vector3& operator*=(KamataEngine::Vector3& v, float s);

// Vector3のスカラー掛け算
const KamataEngine::Vector3 operator*(const KamataEngine::Vector3& v, float s);

// Vector3同士の引き算
const KamataEngine::Vector3 operator-(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);

// ベクトルに行列を掛けて変換する関数
KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);

// 法線ベクトルの変換
KamataEngine::Vector3 TransformNormal(const KamataEngine::Vector3& v, const KamataEngine::Matrix4x4& m);

// Vector3同士の引き算（関数版）
inline KamataEngine::Vector3 Subtract(const KamataEngine::Vector3& a, const KamataEngine::Vector3& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

// Vector3の長さ
inline float Vector3Length(const KamataEngine::Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

// Vector3の正規化
inline KamataEngine::Vector3 Normalize(const KamataEngine::Vector3& v) {
	float len = Vector3Length(v);
	if (len == 0.0f) {
		return {0.0f, 0.0f, 0.0f};
	}
	return {v.x / len, v.y / len, v.z / len};
}
