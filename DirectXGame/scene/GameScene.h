#pragma once
#include<sstream>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugCamera.h"
#include "Enemy.h"
#include "Player.h"
#include "CollisionManager.h"
#include "Skydome.h"
#include"RailCamera.h"
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private: // メンバ変数
	//包含
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 包含
	RailCamera* railCamera_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	CollisionManager* collisionManager_ = nullptr;
	Skydome* skyDome_ = nullptr;

	std::stringstream enemyPopCommands;//敵発生コマンド
	bool isWaiting_=false;
	int32_t waitTimer_=0;
	std::list<Enemy*> enemys_; // 敵
	std::list<EnemyBullet*> enemyBullets_; // 弾
	

DebugCamera* debugCamera_ = nullptr;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

private:

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵を追加
	/// </summary>
	void AddEnemy(const Vector3& pos);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void ColligionTargettoEnemy();
};
