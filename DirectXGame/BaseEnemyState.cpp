#include "BaseEnemyState.h"
#include<Windows.h>

void Log(const std::string& message) { OutputDebugStringA(message.c_str()); }


BaseEnemyState::~BaseEnemyState(){
	
}

void BaseEnemyState::showState() { 
	if (isShowState_ == false) {
		Log(name_);
		isShowState_ = true;
	}
}