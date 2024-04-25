#pragma once
#include<string>

//前方宣言
class Enemy;

class BaseEnemyState {
protected:
	//状態名
	std::string name_;
	//捜査対象の敵
	Enemy* enemy_ = nullptr;
	//ログを一回出す為のフラグ
 	bool isShowState_=false;

public:
	BaseEnemyState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy) {}
virtual	~BaseEnemyState();
	virtual void Update()=0;
	virtual void showState();
};
