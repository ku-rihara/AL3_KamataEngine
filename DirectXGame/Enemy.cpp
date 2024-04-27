#include "Enemy.h"
#include "Geometry/fMatrix4x4.h"
#include "TextureManager.h"
#include "assert.h"


Enemy::Enemy() {}

Enemy::~Enemy() {
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
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
}

void Enemy::Update() {
	state_->Update();

	//終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* enemybullet) {
		if (enemybullet->GetIsDeath()) {
			delete enemybullet;
			return true;
		}
		return false;
	});
	//弾更新
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
	//タイマー更新
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection);
	}
}
//関数**********************************************************************************
void Enemy::Fire() {
	if (enemyBullet_) {
		delete enemyBullet_;
		enemyBullet_ = nullptr;
	}
	//弾を生成して、初期化
	const float kBulletSpeed = -0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Init(model_, worldTransform_.translation_,velocity);

	//弾を登録する
	enemyBullets_.push_back(newBullet);
}

void Enemy::FireAndReset() { 
	Fire();
	//発射タイマーをセットする
	std::function<void(void)> callBack = std::bind(&Enemy::FireAndReset, this);
	//時限発動イベントを生成
	TimedCall* timedCall = new TimedCall(callBack,kFireInterval);
	//時限発動イベントを時限発動イベントリストに追加
	timedCalls_.push_back(timedCall);
}

void Enemy::ApproachInit() {
	// 発射タイマーを初期化
	FireAndReset();
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