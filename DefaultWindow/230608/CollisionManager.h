#pragma once
#include "stdafx.h"
#include "Define.h"
class CObj;

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

public:
	void Initialize();
	static void Update(list<CObj*> _firstList, list<CObj*> _secondList);
	void Release();



	static DIR Collision_Enter_SS(RECT* _first, RECT* _second);
	static DIR Collision_Enter_EE(INFO* _first, INFO* _second);
	static DIR Collision_Enter_ES(INFO* _first, RECT* _second);
	static DIR Collision_Enter_SE(RECT* _second, INFO* _first);




};

