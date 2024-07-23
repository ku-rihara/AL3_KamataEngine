#pragma once
class Colider {
private:
	//衝突半径
	float radius_ = 1.5f;

public:
	Colider();
	~Colider();
	//衝突時に呼ばれる関数
	virtual void OnCollision(){};
	//半径の取得
	float GetRadius() const { return radius_; }
	//半径を設定
	void SetRadius(float radius) { radius_ = radius; }
};
