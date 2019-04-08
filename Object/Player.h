#pragma once
#include "Object.h"

class Input;

class Player :
	public Object
{
public:
	Player();
	Player(int groundLine);
	~Player();
	void InitAnim(void);
	void AddAnim(std::string animName, const Vector2 & id,
				 int frame, int interval);
	void Update(const Input & p);
	void Draw();
private:
	void ChangeAnim();
	void Anim();
	void Idle(const Input & p);
	void Run(const Input & p);
	void Jump(const Input & p);

	void (Player::*updater)(const Input & p);

	void DebugDraw();

	bool runFlag;		// true:走っている, false:走っていない
	bool jumpFlag;		// true:飛んでいる, false:飛んでいない
	bool groundFlag;	// true:地上, false:空中
	bool dieFlag;		// true:生きている, false:死んでいる
    bool turnFlag;		// true:右向き, false:左向き
};

