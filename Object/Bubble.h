#pragma once
#include "CharactorObject.h"

class Bubble;

class Bubble :
	public CharactorObject
{
public:
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	void DebugDraw();
	bool HitPlayer(const bool hitFlag);
	bool HitEnemy(const bool hitFlag);
	bool HitObject(const bool hitFlag);
	bool HitBubble(const bool hitFlag);
	bool CheckShotState();
	bool CheckDelete();
	Rect GetRect();
	Rect ShotGetRect();
	
private:
	void Shot();		// 泡を吐いた時
	void Floating();	// 泡が浮いている時	
	void Pop();


	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	void (Bubble::*updater)();

	bool bubbleDir;		// true:左方向, false:右方向
	bool deleteFlag;	// true:削除, false:削除しない

	int bubbleImage;
	int invCnt;

	const int spitFrame;
};