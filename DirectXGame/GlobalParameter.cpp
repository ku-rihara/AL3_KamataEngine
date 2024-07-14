#include "GlobalParameter.h"
#include <imgui.h>
#include "WinApp.h"
#include <fstream>

template void GlobalParameter::SetValue<int>(const std::string& groupName, const std::string& key, int value);
template void GlobalParameter::SetValue<float>(const std::string& groupName, const std::string& key, float value);
template void GlobalParameter::SetValue<Vector3>(const std::string& groupName, const std::string& key, Vector3 value);

// シングルトンインスタンス
GlobalParameter* GlobalParameter::GetInstance() {
	static GlobalParameter instance;
	return &instance;
}

void GlobalParameter::CreateGroup(const std::string& groupName) {
	// 指定名のオブジェクトが無ければ追加する
	datas_[groupName];
}

void GlobalParameter::Update() {

	if (!ImGui::Begin("Global Parameter", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	// 各グループ
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end(); ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;
		for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目名の参照を取得
			Item& item = itItem->second;
			// int32_t型の値を肘していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			// float型の値を肘していれば
			else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);
			}

			// Vector3型の値を肘していれば
			else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}
		// 改行
		ImGui::Text("\n");
		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalParameter", 0);
		}
		ImGui::EndMenu();
	}
	

	ImGui::EndMenuBar();
	ImGui::End();
}

template<typename T> void GlobalParameter::SetValue(const std::string& groupName, const std::string& key, T value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
void GlobalParameter::SaveFile(const std::string& groupName) {
	// グル―プを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	// 未登録をチェック
	assert(itGroup != datas_.end());

	json root;

	root = json::object();

	// jsonオブジェクト登録
	root[groupName] = json::object();

	// 各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}

		// float型の値を保持していれば
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}

		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({value.x, value.y, value.z});
		}
	}
	//ディレクトリが無ければ作成する
	std::filesystem::path dri(kDirectoryPath);
	if (!std::filesystem::exists(dri)) {
		std::filesystem::create_directories(dri);
	}
	//書き込むJSONファイルのフルパスを合成
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込み用ファイルストリーム
	std::ofstream ofs;
	//ファイル書き込み用に開く
	ofs.open(filePath);
	//ファイルオープン失敗
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	//ファイルにjson文字列を書き込む
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}