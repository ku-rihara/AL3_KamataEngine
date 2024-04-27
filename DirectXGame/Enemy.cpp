#include "Enemy.h"
#include"Player.h"
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
	assert(player_);
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 playerPos = player_->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferentialVector = playerPos - enemyPos;//敵からプレイやー
	Vector3 velocity = Normnalize(DifferentialVector) * kBulletSpeed;

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

//getter-----------------------------------------------------------------
Vector3 Enemy::GetWorldPos() {
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}