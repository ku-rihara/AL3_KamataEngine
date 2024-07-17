#pragma once
#include<memory>
#include"Sprite.h"
class LockOn {
private:
	//ロックオンマーク用スプライト
	std::unique_ptr<Sprite> lockOnMark_;

public:
	void Init();
	void Update();
	void Draw();

};
