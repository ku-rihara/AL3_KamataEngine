#pragma once
#include "Vector3.h"

#include <map>
#include <string>
#include <variant>
#include<json.hpp>

class GlobalParameter {
private:
	struct Item { // 項目の値
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group { // グループ
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;
	//Json
	using json=nlohmann::json;
	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalParameter/";

public:
	// シングルトン
	static GlobalParameter* GetInstance();
	void Update();
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="グループ名"></param>
	/// <param name="key"></param>
	/// <param name="値"></param>
	template<typename T> void SetValue(const std::string& groupName, const std::string& key, T value);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	void SaveFile(const std::string& groupName);

private:
	GlobalParameter() = default;
	~GlobalParameter() = default;
	GlobalParameter(const GlobalParameter&) = delete;
	GlobalParameter& operator=(const GlobalParameter&) = delete;
};
