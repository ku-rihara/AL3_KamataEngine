#include "TimedCall.h"


void TimedCall::Update() {
	if (isFinished_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished_ = true;
		//コールバック関数呼び出し
		callback_();
		
	}
}