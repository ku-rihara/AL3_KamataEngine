#pragma once

#include<memory>
#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
//class
#include "player.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	uint32_t textureHandle_ = 0;   // テクスチャハンドル
	uint32_t soundDataHandle_ = 0; // サウンドデータハンドル
	uint32_t voiceHandle_ = 0;     // 音声再生ハンドル
	Sprite* sprite_ = nullptr;     // スプライト
	std::unique_ptr<Model> model_ = nullptr; // 3Dモデル
	/// <summary>
	/// 包含
	/// </summary>
	std::unique_ptr<Player> player_;

	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();



	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


};
