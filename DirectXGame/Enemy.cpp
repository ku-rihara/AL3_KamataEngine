#include "Enemy.h"
#include"GameScene.h"
#include"Player.h"
#include "Geometry/fMatrix4x4.h"
#include "TextureManager.h"
#include "assert.h"
#include"CollisionConfig.h"

Enemy::Enemy() {  }

Enemy::~Enemy() {

	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
	
}

void Enemy::Init(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("sample.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
	ChangeState(std::make_unique<EnemyStateApproach>(this));
	//接近フェーズ初期化
	ApproachInit();
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
}

void Enemy::Update(const ViewProjection& viewProjection) {
	
	state_->Update();
	
	//終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});

	worldTransform_.UpdateMatrix();

	Vector3 positionReticle = GetWorldPos();
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewPrijectionViewPort = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// ワールド➩スクリーン変換（3Dから2D）
	positionReticle = Transform(positionReticle, matViewPrijectionViewPort);
	// スプライトのレティクルに座標変換
	ScreenPos_ = {positionReticle.x, positionReticle.y};
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
}
//関数**********************************************************************************
void Enemy::Fire() {
	if (enemyBullet_) {
		delete enemyBullet_;
		enemyBullet_ = nullptr;
	}
	assert(player_);
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 playerPos = player_->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferentialVector = playerPos - enemyPos;//敵からプレイやー
	Vector3 velocity = Normnalize(DifferentialVector) * kBulletSpeed;

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Init(model_, worldTransform_.translation_,velocity);
	gameScene_->AddEnemyBullet(newBullet);
	
}

void Enemy::FireAndReset() { 
	
	//発射タイマーをセットする
	std::function<void(void)> callBack = std::bind(&Enemy::FireAndReset, this);
	//時限発動イベントを生成
	TimedCall* timedCall = new TimedCall(callBack,kFireInterval);
	//時限発動イベントを時限発動イベントリストに追加
	timedCalls_.push_back(timedCall);	
}

void Enemy::ApproachInit() {
	// 発射タイマーをセットする
	std::function<void(void)> callBack = std::bind(&Enemy::FireAndReset, this);
	// 時限発動イベントを生成
	TimedCall* timedCall = new TimedCall(callBack, kFireInterval);
	// 時限発動イベントを時限発動イベントリストに追加
	timedCalls_.push_back(timedCall);
}

    //敵の動き
void Enemy::Move(Vector3 velocity) { 
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ += velocity;
}
//state切り替え
void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) { 
	state_ = std::move(state);
}

void Enemy::OnColligion() {
	isdeath_ = true;

}

//getter-----------------------------------------------------------------
Vector3 Enemy::GetWorldPos() {
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

