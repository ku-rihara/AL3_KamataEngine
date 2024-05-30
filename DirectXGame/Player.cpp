#include "Player.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "cassert"
#include"RailCamera.h"

static XINPUT_STATE joyState;
Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Init(Model* model, uint32_t textureHandle) {
	input_ = Input::GetInstance();
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_ = {0, 0, 25};
	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();
	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("anchorPoint.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2(640, 320), Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f));
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void Player::Update(const ViewProjection& viewProjection) {
	// ですフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// マウス座標取得
	POINT mousePos;
	GetCursorPos(&mousePos);
	// クライアントエリアに座標を変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos);
	// スプライトのレティクルに座標変換
	// ゲームパッド
	Vector2 spritePos = sprite2DReticle_->GetPosition();
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePos.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(spritePos);
	} else {
		sprite2DReticle_->SetPosition(Vector2(float(mousePos.x), float(mousePos.y)));
	}

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// VPV行列
	Matrix4x4 matVPV = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	// スクリーン座標
	Vector3 posNear = Vector3(float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 0);
	Vector3 posFar = Vector3(float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 1);
	// スクリーンからワールド
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normnalize(mouseDirection);
	// カメラから標準オブジェクトの距離
	float kDistanceTestObject =150;
	worldTransform3DReticle_.translation_.x = posNear.x+ mouseDirection.x * kDistanceTestObject;
	worldTransform3DReticle_.translation_.y = posNear.y + mouseDirection.y * kDistanceTestObject;
	worldTransform3DReticle_.translation_.z = posNear.z + mouseDirection.z * kDistanceTestObject;
	worldTransform3DReticle_.UpdateMatrix();

	// キャラクター旋回処理
	Rotate();
	// キャラクターの移動処理
	Move(viewProjection);
	// キャラクターの攻撃処理
	Attack();
	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	};

	worldTransform_.UpdateMatrix(); // 行列更新

	ImGui::Begin("player");
	ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() {
	// 3Dレティクル
	sprite2DReticle_->Draw();
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Move(const ViewProjection& viewProjection) {
	Vector3 move = {0, 0, 0};
	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	// ゲームパッド
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	worldTransform_.translation_ += move;
	viewProjection;
	// 移動限界座標
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 11.0f;
	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x,  kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
}

void Player::Attack() {

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || (!Input::GetInstance()->GetJoystickState(0, joyState) && Input::GetInstance()->TriggerKey(DIK_W))) {
		// 弾の速度
		const float kBulletSpeed = 5.0f;
		/*Vector3 velocity = {0.0f, 0.0f, 1.0f};*/
		//	 速度ベクトルヲ自機の向きに合わせて回転させる
			
		// 自機から標準オブジェクトへのベクトル
	Vector3 velocity = GetWorld3DRecticlPos() - GetWorldPos();
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		velocity = Normnalize(velocity) * kBulletSpeed;
		newBullet->Init(model_, GetWorldPos(), velocity);
		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::OnColligion() {
	// 何もしない
}

// Getter-------------------------------------------------------------
Vector3 Player::GetWorldPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorld3DRecticlPos() {
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

// setter
// 親子関係を結ぶ
void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }