#pragma once

#include<memory>
#include<vector>
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
#include "SkyDome.h"
#include "Ground.h"
#include "FollowCamera.h"

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
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	std::unique_ptr<Model> modelSkyDome_ = nullptr; // 3Dモデル
	std::unique_ptr<Model> modelGround_ = nullptr; // 3Dモデル
	/*std::vector<std::unique_ptr<Model>> modelFighterParts;*/
	std::unique_ptr<Model> modelFighterBody_ = nullptr;        // 3Dモデル
	std::unique_ptr<Model> modelFighterHead_ = nullptr; // 3Dモデル
	std::unique_ptr<Model> modelFighterLeftArm_ = nullptr;        // 3Dモデル
	std::unique_ptr<Model> modelFighterRightArm_ = nullptr; // 3Dモデル
	/// <summary>
	/// 包含
	/// </summary>
	std::unique_ptr<Player> player_;
	std::unique_ptr<Skydome> skyDome_;
	std::unique_ptr<Ground> ground_;
	std::unique_ptr<FollowCamera> followCamera_;
	/// <summary>
	/// メンバ変数
	/// </summary>
	bool isDebugCameraActive_ = false;

	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
	
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
