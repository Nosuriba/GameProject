#pragma once
#include "StageObject.h"
class Wall :
	public StageObject
{
public:
	Wall();
	~Wall();
	void Update();
	void Draw();
	Rect GetRect();
private:
};

