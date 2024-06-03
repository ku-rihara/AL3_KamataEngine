#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}

	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	delete model_;
	delete railCamera_;
	delete modelSkydome_;
	delete skyDome_;
	delete debugCamera_;
	delete player_;
	delete collisionManager_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// インスタンス生成
	skyDome_ = new Skydome();
	player_ = new Player();
	railCamera_ = new RailCamera();
	collisionManager_ = new CollisionManager();
	debugCamera_ = new DebugCamera(1280, 720);
	// 画像読み込み
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	textureHandle_ = TextureManager::Load("white1x1.png");
	TextureManager::Load("anchorPoint.png");
	LoadEnemyPopData();
	// モデル作成
	model_ = Model::Create();
	// 初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	railCamera_->Init(Vector3{0, 0, 0}, Vector3{0, 0, 0});
	player_->Init(model_, textureHandle_);
	skyDome_->Init(modelSkydome_);
	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetGameScene(this);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	UpdateEnemyPopCommands();
	railCamera_->Update();
	skyDome_->Update();
	player_->Update(viewProjection_);

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* enemybullet) {
		if (enemybullet->GetIsDeath()) {
			delete enemybullet;
			return true;
		}
		return false;
	});

	// デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDeath()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 敵更新
	for (Enemy* enemy : enemys_) {

		enemy->Update(viewProjection_);
	}
	// 弾更新
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;


	collisionManager_->ClearList();

	collisionManager_->EntryList(player_);
	for (Enemy* enemy : enemys_) {
		collisionManager_->EntryList(enemy);
	}
	for (EnemyBullet* eBullet : enemyBullets) {
		collisionManager_->EntryList(eBullet);
	}
	for (PlayerBullet* pBullet : playerBullets) {
		collisionManager_->EntryList(pBullet);
	}
	
		ColligionTargettoEnemy();
	
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

	// 敵描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}
	// 弾描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
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
	player_->DrawUI();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(const Vector3& pos) {

	enemy_ = new Enemy();
	enemy_->Init(model_, pos, Vector3{0, 0, 0.1f});
	enemy_->SetPlayer(player_);
	enemy_->SetGameScene(this);
	enemys_.push_back(enemy_);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/EnemyPos.csv");
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();
	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaiting_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			// 待機完了
			isWaiting_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメントは飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;
			waitTimer_ = waitTime;
			// ループを抜ける
			break;
		}
	}
}



void GameScene::ColligionTargettoEnemy() {
	bool isLockedOn = false;
	
	for (Enemy* enemy : enemys_) {
		enemy->SetIsTarget(false);
		Vector2 posA = player_->Get2DreticlePos();
		float distance = powf((posA.x - enemy->GetScreenPos().x), 2) + powf((posA.y - enemy->GetScreenPos().y), 2);

		// 球と球の交差判定
		if (distance <= 144 * 30) { // 144の半径を使った円の交差判定
			isLockedOn = true;
			enemy->SetIsTarget(true);
			break;
		}	
	}

	if (isLockedOn) {
		player_->RockOn();
	} else {
		player_->CanselRockOn();
	}
}
//const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
//
//// コライダー
//std::list<Collider*> colliders_;
//
//colliders_.push_back(enemy_);
//
//for (PlayerBullet* pBullet : playerBullets) {
//	colliders_.push_back(pBullet);
//}
//
//// リスト内のペアを総当たり
//std::list<Collider*>::iterator itrA = colliders_.begin();
//for (; itrA != colliders_.end(); ++itrA) {
//	// イテレータAからコライダーAを取得する
//	Collider* colliderA = *itrA;
//
//	// イテレーターBはイテレータAの次の要素から回す（重複判定を回避）
//	std::list<Collider*>::iterator itrB = itrA;
//	itrB++;
//	for (; itrB != colliders_.end(); ++itrB) {
//		Collider* colliderB = *itrB;
//
//		// ペアの当たり判定
//		CheckCollisionPair(colliderA, colliderB);
//	}