#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	delete skyDome_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
	delete collisionManager_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// インスタンス生成
	skyDome_ = new Skydome();
	player_ = new Player();
	enemy_ = new Enemy();
	collisionManager_ = new CollisionManager();
	debugCamera_ = new DebugCamera(1280, 720);
	// 画像読み込み
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome",true);
	textureHandle_ = TextureManager::Load("white1x1.png");
	// モデル作成
	model_ = Model::Create();
	// 初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	player_->Init(model_, textureHandle_);
	enemy_->Init(model_, Vector3{10, 2, 20}, Vector3{0, 0, 0.1f});
	skyDome_->Init(modelSkydome_);
	// 自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	skyDome_->Update();
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	collisionManager_->ClearList();

	collisionManager_->EntryList(player_);
	collisionManager_->EntryList(enemy_);
	for (EnemyBullet* eBullet : enemyBullets) {
		collisionManager_->EntryList(eBullet);
	}
	for (PlayerBullet* pBullet : playerBullets) {
		collisionManager_->EntryList(pBullet);
	}

	collisionManager_->ChecAllCollisions();
#ifdef _DEBUG
	// デバッグカメラモード切り替え------------------------------
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_ == true) {
			isDebugCameraActive_ = false;
		}
	}
	// デバッグカメラモード切り替え------------------------------
#endif
	if (isDebugCameraActive_ == true) { // デバッグカメラがアクティブなら
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();

	} else { // アクティブでない
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skyDome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

