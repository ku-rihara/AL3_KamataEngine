#pragma once
#include "Vector3.h"

#include <map>
#include <string>
#include <variant>


class GlobalParameter {
private:
	struct Item { // 項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group { // グループ
		std::map<std::string, Item> items;
	};

public:
	// 全データ
	std::map<std::string, Group> datas_;

public:
	// シングルトン
	static GlobalParameter* GetInstance();
	void Update();
	void CreateGroup(const std::string& groupName);

	// 値のセット

	template<typename T> 
	void SetValue(const std::string& groupName, const std::string& key, T value);

private:
	GlobalParameter() = default;
	~GlobalParameter() = default;
	GlobalParameter(const GlobalParameter&) = delete;
	GlobalParameter& operator=(const GlobalParameter&) = delete;
};
