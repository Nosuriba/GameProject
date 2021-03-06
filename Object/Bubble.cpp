#include <DxLib.h>
#include "../Game.h"
#include "../CollisionDetector.h"
#include "../Input.h"
#include "../AudioMng.h"
#include "Bubble.h"

const int spitFrame		= 10;		// 泡を吐く間隔
const float bubbleSpeed = 0.5f;		// 泡の速度
const float colSpeed	= 1.5f;		// 泡同士が接触した時の速度

Bubble::Bubble()
{
	Floating();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");
	bblImage	= DxLib::LoadGraph(actionData.imgFilePath.c_str());

	isGame		= isDelete = false;
	vel.y	    = -(0.4f * (GetRand(6) + 1));
	invCnt      = spitFrame;
}

Bubble::Bubble(const bool& bubbleDir)
{
	Shot();
	nowCutIdx = 0;
	ReadActionFile("Action/bubble.act");
	bblImage = DxLib::LoadGraph(actionData.imgFilePath.c_str());

	this->bubbleDir = bubbleDir;
	isGame			= true;
	isDelete		= false;
	invCnt			= spitFrame;
}

Bubble::~Bubble()
{
	DxLib::DeleteGraph(bblImage);
}

const Vector2f & Bubble::GetPos()
{
	return pos;
}

Rect Bubble::GetRect()
{
	auto center   = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x, size.y);

	return Rect(center, rectSize);
}

Rect Bubble::ShotGetRect()
{
	auto center   = Vector2(pos.x + (size.x / 2), pos.y + (size.x / 2));
	auto rectSize = Size(size.x / 2, size.y / 2);

	return Rect(center, rectSize);
}

bool Bubble::IsDelete()
{
	return isDelete;
}

void Bubble::HitAcross(const Rect & pRect, const Rect& wRect)
{
	if (updater == &Bubble::FloatingUpdate)
	{
		SideCheck(pRect, wRect);
	}
}

void Bubble::MoveContact(const Rect & bblRect)
{
	auto hitCheck  = (CollisionDetector::CollCheck(GetRect(), bblRect));
	auto sideCheck = (CollisionDetector::SideCollCheck(GetRect(), bblRect));

	if (updater == &Bubble::FloatingUpdate)
	{
		/// 泡同士が当たった時の挙動(横移動)
		if (GetRect().Right() - (size.x / 3) > bblRect.Left()	&&
			GetRect().Right() - (size.x / 3) < bblRect.center.x &&
			sideCheck)
		{
			vel.x = -bubbleSpeed;
		}
		else if (GetRect().Left() + (size.x / 3) < bblRect.Right()	&&
				 GetRect().Left() + (size.x / 3) > bblRect.center.x &&
				 sideCheck)
		{
			vel.x = bubbleSpeed;
		}
		else
		{
			vel.x = 0;
		}

		/// 泡同士が当たった時の挙動(縦移動)
		if (GetRect().Bottom() > bblRect.center.y &&
			GetRect().Bottom() < bblRect.center.y + (size.y / 3) &&
			hitCheck)
		{
			vel.y = -bubbleSpeed * 2;
			return;
		}
		else if (GetRect().Top() < bblRect.center.y &&
				 GetRect().Top() > bblRect.center.y - (size.y / 3) &&
				 hitCheck)
		{
			vel.y = bubbleSpeed;
			return;
		}
		else
		{
			vel.y = -bubbleSpeed;
		}
	}
}

bool Bubble::HitPlayer(const Rect &pRect, const Input & p)
{
	if (updater == &Bubble::FloatingUpdate)
	{
		return UnderCheck(pRect, p);
	}
	return false;
}

bool Bubble::HitEnemy(const Rect& eRect)
{
	auto hitCheck = (CollisionDetector::CollCheck(GetRect(), eRect));
	if (updater == &Bubble::ShotUpdate && hitCheck)
	{
		isDelete = false;
		return true;
	}
	return false;
}

bool Bubble::HitObject(const Rect& objRect)
{
	auto selHitCheck = [&](const Rect& objRect)		
	{
		if (objRect.size.height == size.y)
		{
			/// 壁との当たり判定
			return CollisionDetector::SideCollCheck(GetRect(), objRect);
		}
		else
		{
			/// ブロックとの当たり判定
			return CollisionDetector::SideCollCheck(ShotGetRect(), objRect);
		}
	};
	
	if (updater == &Bubble::ShotUpdate && selHitCheck(objRect))
	{
		Floating();
		return true;
	}
	return false;
}

bool Bubble::HitBubble(const Rect& bblRect)
{
	auto hitCheck = CollisionDetector::CollCheck(GetRect(), bblRect);

	if (updater == &Bubble::PopUpdate && hitCheck)
	{
		/// 泡が破裂している時に入る
		vel = Vector2f(0, 0);
		return true;
	}
	return false;
}

void Bubble::ChangePop()
{
	if (updater != &Bubble::PopUpdate)
	{ 
		AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
		AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);

		Pop(); 
	}
}

void Bubble::SideCheck(const Rect & pRect, const Rect& wRect)
{
	auto hitCheck  = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto sideCheck = (CollisionDetector::SideCollCheck(GetRect(), wRect));
	
	if (hitCheck)
	{
		/// 壁に当たった時、泡を割る
		if (sideCheck)
		{
			AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
			AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);
			Pop();
			return;
		}
		else
		{
			if (GetRect().center.x + (size.x / 2) > pRect.center.x - (pRect.size.width / 2) &&
				GetRect().center.x + (size.x / 2) < pRect.center.x)
			{
				/// プレイヤーの左側と当たった時の挙動
				vel.x = -bubbleSpeed;
			}
			else if (GetRect().center.x - (size.x / 2) < pRect.center.x + (pRect.size.width / 2) &&
					 GetRect().center.x - (size.x / 2) > pRect.center.x)
			{
				/// プレイヤーの右側と当たった時の挙動
				vel.x = bubbleSpeed;
			}
			else {}
		}
	}
	else
	{
		vel.x = 0;
	}
}

bool Bubble::UnderCheck(const Rect & pRect, const Input & p)
{
	auto hitCheck	 = (CollisionDetector::CollCheck(GetRect(), pRect));
	auto underBubble = (CollisionDetector::UnderCollCheck(GetRect(), pRect));		// 泡の下側との当たり判定
	auto underPlayer = (CollisionDetector::UnderCollCheck(pRect, GetRect()));		// ﾌﾟﾚｲﾔｰの下側との当たり判定

	if (hitCheck)
	{
		if (p.IsPressing(PAD_INPUT_1))
		{
			/// 地上でﾎﾞﾀﾝを押し続けている時に泡に当たると、泡が割れる
			if (underBubble || (GetRect().Top() < pRect.center.y + (size.y / 4)))
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
				AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);
				Pop();
				return false;
			}
			/// ﾎﾞﾀﾝを押し続けていると、泡の上を飛ぶことができる
			if (underPlayer)
			{
				return true;
			}
		}
		else
		{
			if (underPlayer)
			{
				AudioMng::GetInstance().PlaySE(AudioMng::GetInstance().GetSound().pop);
				AudioMng::GetInstance().ChangeVolume(200, AudioMng::GetInstance().GetSound().pop);
				Pop();
				return false;
			}
		}
		
	}
	return false;
}

bool Bubble::CeilCheck()
{
	auto rtnFlag = false;
	if (pos.y < (size.y * 2) + (size.y / 2))
	{
		rtnFlag = true;

		if (pos.y < size.y + (size.y / 2))
		{
			vel.y = 0;
		}
	}
	return rtnFlag;
}

void Bubble::Shot()
{
	ChangeAction("shot");
	updater = &Bubble::ShotUpdate;
}

void Bubble::Floating()
{
	vel.x = 0;
	vel.y = -0.5;					
	ChangeAction("floating");
	updater = &Bubble::FloatingUpdate;
}

void Bubble::Pop()
{
	ChangeAction("pop");
	turnFlag = false;				/// ｴﾌｪｸﾄの描画反転を防いている
	updater  = &Bubble::PopUpdate;
}

void Bubble::ShotUpdate()
{
	if (invCnt < 0)
	{
		Floating();
	}
	else
	{
		invCnt--;
		if (bubbleDir)
		{
			vel.x = -20.0;
		}
		else
		{
			vel.x = 20.0;
		}
	}
}

void Bubble::FloatingUpdate()
{
}

void Bubble::PopUpdate()
{
	vel = Vector2f(0, 0);
	isDelete = ProceedAnimFile();
}

void Bubble::Update()
{
	(this->*updater)();
	pos += vel;
}

void Bubble::Draw()
{
#ifdef _DEBUG
	if (gameFlag)
	DebugDraw();
#endif
	CharactorObject::Draw(bblImage);
}

void Bubble::DebugDraw()
{
	DrawString(100, 0, nowActionName.c_str(), 0xffffff);
	if (nowActionName == "floating")
	{
		DrawBox(GetRect().Left(), GetRect().Top(), GetRect().Right(), GetRect().Bottom(), 0xff0000, false);
	}
	else if (nowActionName == "shot")
	{
		DrawBox(ShotGetRect().Left(), ShotGetRect().Top(), ShotGetRect().Right(), ShotGetRect().Bottom(), 0xff0000, false);
	}
	else {}

}
