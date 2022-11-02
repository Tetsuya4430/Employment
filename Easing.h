#pragma once
#include <DirectXMath.h>

class Easing
{
public:
	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="Move">移動量</param>
	/// <param name="BasePos">最初の位置</param>
	/// <param name="MoveTime">移動時間</param>
	/// <param name="Time">現在の経過時間</param>
	/// <returns></returns>
	static float EaseIn(float Move, float BasePos, float MoveTime, float Time);
};

