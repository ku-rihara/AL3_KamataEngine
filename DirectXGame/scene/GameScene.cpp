#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();//自キャラ生成
	debugCamera_ = new DebugCamera(1280, 720);
	textureHandle_ = TextureManager::Load("white1x1.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	player_->Init(model_, textureHandle_); // 自キャラ初期化
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {
	player_->Update();
#ifdef _DEBUG 
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isBebugCameraActive_ == false) {
			isBebugCameraActive_ = true;
		}
		else if (isBebugCameraActive_ == true)	{
			isBebugCameraActive_ = false;
		}
	}
#endif
	if (isBebugCameraActive_ == true) {
		// デバッグカメラの更新
		debugCamera_->Update();
		
		viewProjection_.UpdateViewMatrix();
		/*viewProjection_.UpdateProjectionMatrix();
		viewProjection_.matView = debugCamera_->GetViewProjection();*/
	} else {
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
