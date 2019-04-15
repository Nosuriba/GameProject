#pragma once
#include "CharactorObject.h"

class Bubble :
	public CharactorObject
{
public:
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	Rect GetRect();
private:
	void Spit();		// 泡を吐いた時
	void Floating();	// 泡が浮いている時	

	void SpitUpdate();
	void FloatingUpdate();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:左方向, false:右方向

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};