#include "ShakeManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include <random>
#ifdef _DEBUG
#include"imgui.h"
#endif // DEBUG


ShakeManager& ShakeManager::GetInstance() {
	static ShakeManager instance;
	return instance;
}

//2Dのシェイク追加
void ShakeManager::AddShake(Vector2& position, int groupId) {
	shake2D.push_back({&position, position, 0, 0.0f, groupId}); }
// 3Dのシェイク追加
void ShakeManager::AddShake(Vector3& position, int groupId) {
	shake3D.push_back({&position, position, 0, 0.0f, groupId}); }

//グループごとのシェイクのパラメータ設定
void ShakeManager::SetShakeGroupSettings(int groupId, float size, int shakeCount, float shakeInterval) {
	shakeGroupSettings_[groupId] = {size, shakeCount, shakeInterval}; 
}

//更新
void ShakeManager::Update(int groupId, float& shakeTime) {
	//指定されたグループのシェイクを取得
	auto it = shakeGroupSettings_.find(groupId);
	if (it == shakeGroupSettings_.end())
		return;
	//シェイクグループの設定を取得
	const ShakeGroupSettings& settings = it->second;

	//2D更新
	for (auto& shakeData : shake2D) {
		if (shakeData.groupId == groupId) {
			if (shakeData.shakeCount == 0) {
				shakeData.originalPos = *shakeData.pos;
				shakeData.shakeCount = settings.shakeCount;
			}
			// シェイクを更新
			UpdateShake(*shakeData.pos, settings, shakeTime, shakeData.lastShakeTime, shakeData.originalPos);
		}
	}
	
	//3D更新
	for (auto& shakeData : shake3D) {
		if (shakeData.groupId == groupId) {
			if (shakeData.shakeCount == 0) {
				shakeData.originalPos = *shakeData.pos;
				shakeData.shakeCount = settings.shakeCount;		
			}
			// シェイクを更新
			UpdateShake(*shakeData.pos, settings, shakeTime, shakeData.lastShakeTime, shakeData.originalPos);

		}
		
	}
	
}

template<typename T> void ShakeManager::UpdateShake(T& position, const ShakeGroupSettings& settings, float& shakeTime, float& lastShakeTime, T& originalPosition) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	
	// 現在のシェイク回数を計算
	int currentShake = int(shakeTime / settings.shakeInterval);

	if (currentShake >= settings.shakeCount) {
		position = originalPosition;
		lastShakeTime = 0;
		return;
	}
	// 前回のシェイクから設定間隔が経過した場合
	if (shakeTime - lastShakeTime >= settings.shakeInterval) {
		float currentAmplitude = settings.shakeSize * (1.0f - (float(currentShake) / float(settings.shakeCount)));

		float offsetX = float(dis(gen)) * currentAmplitude;
		float offsetY = float(dis(gen)) * currentAmplitude;
		float offsetZ = float(dis(gen)) * currentAmplitude;

		if constexpr (std::is_same_v<T, Vector2>) {
			position.x = originalPosition.x + offsetX;
			position.y = originalPosition.y + offsetY;
		} else if constexpr (std::is_same_v<T, Vector3>) {
			position.x = originalPosition.x + offsetX;
			position.y = originalPosition.y + offsetY;
			position.z = originalPosition.z + offsetZ;
		}

		 lastShakeTime = shakeTime; // 最後のシェイク時間を更新
	}
	/*ImGui::Text("Shake Time: %2.1f", shakeTime);
	ImGui::Text("Shake Time: %d", currentShake);*/
}