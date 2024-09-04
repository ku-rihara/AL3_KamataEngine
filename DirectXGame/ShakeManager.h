#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <unordered_map>
#include <vector>

class ShakeManager {
private:
	struct ShakeData2D {
		Vector2* pos;
		Vector2 originalPos;
		int shakeCount;
		float lastShakeTime;
		int groupId;
	};

	struct ShakeData3D {
		Vector3* pos;
		Vector3 originalPos;
		int shakeCount;
		float lastShakeTime;
		int groupId;
	};

	struct ShakeGroupSettings {
		float shakeSize;
		int shakeCount;
		float shakeInterval;
	};

	// グループごとの設定を保存するマップ
	std::unordered_map<int, ShakeGroupSettings> shakeGroupSettings_;

private://メンバ変数***************************************************
	std::vector<ShakeData2D> shake2D;
	std::vector<ShakeData3D> shake3D;

public://メンバ関数**********************************************************
	static ShakeManager& GetInstance();

	/// <summary>
	///シェイクしたい座標のの追加
	/// </summary>
	/// <param name="座標"></param>
	/// <param name="シェイクグループ"></param>
	void AddShake(Vector2& position, int groupId);
	void AddShake(Vector3& position, int groupId);
	
	/// <summary>
	/// シェイク更新
	/// </summary>
	/// <param name="シェイクグループ"></param>
	/// <param name="シェイクタイム"></param>
	void Update(int groupId, float& shakeTime);

	/// <summary>
	/// グループごとのシェイクのパラメータ設定
	/// </summary>
	/// <param name="シェイクグループ"></param>
	/// <param name="サイズ"></param>
	/// <param name="シェイク回数"></param>
	/// <param name="シェイク間隔時間"></param>
	void SetShakeGroupSettings(int groupId, float size, int shakeCount, float shakeInterval);

private:
	ShakeManager() {}

	template<typename T> void UpdateShake(T& position, const ShakeGroupSettings& settings, float& shakeTime, float& lastShakeTime, T& originalPosition);
};
