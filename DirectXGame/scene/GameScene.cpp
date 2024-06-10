#include "GameScene.h"
#include "MathFunction.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete railCamera_;
	delete modelSkydome_;
	delete skyDome_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
}

void GameScene::Initialize() {
	controlPoints_ = {
	    {0, 0, 0},
	    {
         10, 10,
         0, },
	    {10, 15, 0},
	    {20, 15, 0},
	    {20, 0, 0},
	    {30, 0, 0},
	};
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// primitiveDrawer_ = PrimitiveDrawer::GetInstance();
	//
	//  インスタンス生成
	skyDome_ = new Skydome();
	player_ = new Player();
	enemy_ = new Enemy();
	railCamera_ = new RailCamera();
	debugCamera_ = new DebugCamera(1280, 720);
	// 画像読み込み
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	textureHandle_ = TextureManager::Load("white1x1.png");
	// モデル作成
	model_ = Model::Create();
	// 初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	railCamera_->Init(Vector3{0, 0, 0}, Vector3{0, 0, 0});
	player_->Init(model_, textureHandle_);
	enemy_->Init(model_, Vector3{10, 2, 90}, Vector3{0, 0, 0.1f});
	skyDome_->Init(modelSkydome_);
	// 自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	// 軸方向表示の表示を有効にする
	// AxisIndicator::GetInstance()->SetVisible(true);
	//// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	// AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	railCamera_->Update();
	skyDome_->Update();
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}

	ChecAllCollisions();
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
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
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
	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing;
	// 線分の数
	const size_t segmentCount = 100;
	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}
	for (size_t i = 0; i < segmentCount; ++i) {
		// 先頭から2点取り出してライン描画
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[i], pointsDrawing[i + 1], Vector4{1.0f, 0.0f, 0.0f, 1.0f});
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

void GameScene::ChecAllCollisions() {
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	// コライダー
	std::list<Collider*> colliders_;
	// コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);

	for (PlayerBullet* pBullet : playerBullets) {
		colliders_.push_back(pBullet);
	}
	for (EnemyBullet* eBullet : enemyBullets) {
		colliders_.push_back(eBullet);
	}
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレーターBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA, posB;
	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() ^ colliderB->GetCollisionMask()) == 0xFFFFFFFF || (colliderB->GetCollisionAttribute() ^ colliderA->GetCollisionMask()) == 0xFFFFFFFF) {
		return;
	}
	posA = colliderA->GetWorldPos();
	posB = colliderB->GetWorldPos();
	float distaince = powf((posA.x - posB.x), 2) + powf((posA.y - posB.y), 2) + powf((posA.z - posB.z), 2);

	// 球と球の交差判定
	if (distaince <= 5 + 5) {
		// 自キャラ衝突時コールバックを呼び出す
		colliderA->OnColligion();
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnColligion();
	}
}