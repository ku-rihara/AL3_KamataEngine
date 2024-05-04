#pragma once
#include"Vector3.h"
#include <vector>

float Clamp(float n, float min, float max);
size_t Clamp(size_t n, size_t min, size_t max);


/// <summary>///
/// CatmullRomの補間
/// </summary>///
///<param name="p0">点0の座標</param>/// 
/// <param name="p1">点1の座標</param>/// 
/// <param name="p2">点2の座標</param>/// 
/// <param name="p3">点3の座標</param>/// 
/// <param name="t">点1を0.0f、点2を1.0fとした割合指定</param>///
///  <returns>点1と点2の間で指定された座標</returns>
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3 p2, const Vector3& p3, float t);

/// <summary>///
/// CatmullRomスプライン曲線状の座標を得る
/// </summary>///
///  <param name="points">制御点の集合</param>///
///  <param name="t">スプラインの全区間の中での割合指定[0,1]</param>///
///  <returns></returns>
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);