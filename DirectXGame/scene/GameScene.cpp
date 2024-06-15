#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}



void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("white1x1.png");
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	modelSkyDome_.reset(Model::CreateFromOBJ("skydome", true));
	modelGround_.reset(Model::CreateFromOBJ("Ground", true));
	modelFighterParts.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("playerBody", true)));
	modelFighterParts.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("PlayerFace", true)));
	modelFighterParts.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("PlayerLeftArm", true)));
	modelFighterParts.push_back(std::unique_ptr<Model>(Model::CreateFromOBJ("PlayerRightArm", true)));
	/// <summary>
	/// 生成
	/// </summary>
	player_ = std::make_unique<Player>(); 
	skyDome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	followCamera_ = std::make_unique<FollowCamera>();
	/// <summary>
	/// 初期化
	/// </summary>
	for (std::unique_ptr<Model> parts : modelFighterParts) {
		player_->Init(modelFighterParts.get\, textureHandle_); // 自キャラ初期化
	}
	skyDome_->Init(modelSkyDome_.get());
	ground_->Init(modelGround_.get());
	followCamera_->Init();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
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
		// アクティブでない
	} 
	skyDome_->Update();
	ground_->Update();
	followCamera_->Update();
	player_->Update();

	if (isDebugCameraActive_ == false) { // デバッグカメラがアクティブでない
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

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
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
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
