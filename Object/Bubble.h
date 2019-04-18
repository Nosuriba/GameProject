#pragma once
#include "CharactorObject.h"

class Bubble;
class Input;

class Bubble :
	public CharactorObject
{
public:
	Bubble();
	Bubble(const bool& bubbleDir);
	~Bubble();

	void Update();
	void Draw();
	void ChangePop();
	const bool& HitPlayer(const bool& hitFlag, const bool& groundFlag, const Input& p);
	const bool& HitEnemy(const bool& hitFlag);
	const bool& HitObject(const bool& hitFlag);
	const bool& HitBubble(const bool& hitFlag);
	const bool& CheckShotState();
	const bool& CheckPopState();
	const bool& CheckDelete();
	void MoveContact(const Rect& rcB);
	const Vector2f& GetPos();
	Rect GetRect();
	Rect ShotGetRect();
private:
	void Shot();		// 泡を吐いた時
	void Floating();	// 泡が浮いている時	
	void Pop();

	void ShotUpdate();
	void FloatingUpdate();
	void PopUpdate();

	const bool& CeilCheck();
	void DebugDraw();

	void (Bubble::*updater)();

	bool gameFlag;		// true:ゲーム中, false:ゲーム中でない
	bool bubbleDir;		// true:左方向, false:右方向
	bool deleteFlag;	// true:削除, false:削除しない

	int bubbleImage;
	int invCnt;

	const int spitFrame;
	const float defSpeed;
	const float colSpeed;
};