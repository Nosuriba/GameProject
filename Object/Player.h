#pragma once
#include "../Object/CharactorObject.h"

class CollisionDetector;
class Input;

class Player :
	public CharactorObject
{
public:
	Player();
	Player(int groundLine);
	~Player();
	void CheckHit(bool hitFlag);
	void Update(const Input & p);
	void Draw();
	Rect GetRect();
private:
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);
	void Eat(const Input & p);
	void Shot(const Input & p);
	void OnGround();

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	std::string actionName;

	int playerImg;

	bool runFlag;		// true:走っている, false:走っていない
	bool jumpFlag;		// true:飛んでいる, false:飛んでいない
	bool groundFlag;	// true:地上, false:空中
	bool dieFlag;		// true:生きている, false:死んでいる

};

