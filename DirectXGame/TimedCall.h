#pragma once
#include<functional>
class TimedCall {
private:
	//コールバック
	std::function<void(void)> callback_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinished_=false;

public: 

	//コンストラクタ
	TimedCall(std::function<void(void)> callback, uint32_t time): callback_(callback),time_(time){};
	//更新
	void Update();
	//完了ならtrueを返す
	bool IsFinished() { return isFinished_; }

};
