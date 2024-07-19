#include "LockOn.h"
#include "Enemy.h"
#include "Geometry/fMatrix4x4.h"
#include "TextureManager.h"
#include "input/Input.h"

void LockOn::Init() {
	int TextureHandle = TextureManager::Load("./Resources/anchorPoint.png");

	lockOnMark_.reset(Sprite::Create(TextureHandle, Vector2{640,320},Vector4(1,1,1,1), Vector2(0.5f, 0.5f)));
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	// ロックオンをトリガー
	if (/*!isRockOn_ &&*/ Input::GetInstance()->TriggerKey(DIK_R)) {
		// ロックオン対象の検索
		Search(enemies, viewProjection);
		/*isRockOn_ = true;*/
	}
	
}

void LockOn::Draw() {

	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	// 目標
	std::list<std::pair<float, const Enemy*>> targets;
	// 全ての敵に対して順にロックオンを判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		// 敵のロックオン座標を取得
		Vector3 positionWorld = enemy->GetCenterPos();
		// ワールド→ビュー座標系
		Vector3 positionView = Transform(positionWorld, viewProjection.matView);
		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			// カメラ前方との角度を計算
			float actTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),positionView.z);

			//角度条件チェック（コーンに収まっているか）
			if (std::fabsf(actTangent) <= std::fabsf(angleRange_)) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
		// ロックオン対象をリセット
		target_ = nullptr;
		if (!targets.empty()) {
			//距離で昇順にソート
			targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
			//ソートの結果一番近い敵をロックオン対象とする
			target_ = targets.front().second;
		}
	}
}