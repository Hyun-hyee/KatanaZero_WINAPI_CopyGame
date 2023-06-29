#include "stdafx.h"
#include "CollisionManager.h"
#include "Obj.h"

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Initialize()
{
}

void CCollisionManager::Update(list<CObj*> _firstList, list<CObj*> _secondList)
{
	DIR CollisionDIR = DIR_NONE;
	int ObjEvent = OBJ_NOEVENT;
	
	INFO* fir;
	RECT* sec;

	for (auto& iter : _firstList)
	{

		for (auto& iter2 : _secondList)
		{
			if (iter != iter2)
			{
				if (iter->Get_Collider_type() == RECTANGLE && iter2->Get_Collider_type() == RECTANGLE)
					CollisionDIR = Collision_Enter_SS(iter->Get_Collide(), iter2->Get_Collide());
				else if (iter->Get_Collider_type() == ELLIPSE && iter2->Get_Collider_type() == ELLIPSE)
					CollisionDIR = Collision_Enter_EE(iter->Get_Info(), iter2->Get_Info());
				else if (iter->Get_Collider_type() == RECTANGLE && iter2->Get_Collider_type() == ELLIPSE)
					CollisionDIR = Collision_Enter_SE(iter->Get_Collide(), iter2->Get_Info());
				else if (iter->Get_Collider_type() == ELLIPSE && iter2->Get_Collider_type() == RECTANGLE)
					CollisionDIR = Collision_Enter_ES(iter->Get_Info(), iter2->Get_Collide());
				
				if (CollisionDIR != DIR_NONE)
					ObjEvent = (iter)->OnCollision((iter2), CollisionDIR);

				switch (ObjEvent)
				{
				case OBJ_NOEVENT:
					break;
				}
				ObjEvent = OBJ_NOEVENT;
				CollisionDIR = DIR_NONE;
			}
		}
	}
}

void CCollisionManager::Release()
{
}

DIR CCollisionManager::Collision_Enter_SS(RECT* _first, RECT* _second)
{
	RECT section;
	if (IntersectRect(&section, _first, _second))
	{
		if (section.right == _second->right)
			return LEFT;
		else if (section.left == _second->left)
			return RIGHT;
		else if (section.top == _second->top)
			return BOTTOM;
		else if (section.bottom == _second->bottom)
			return TOP;
	}
	else
		return DIR_NONE;

}

DIR CCollisionManager::Collision_Enter_EE(INFO* _first, INFO* _second)
{
	float Distance = sqrt(pow((_first->fX - _second->fX), 2.f) + pow((_first->fY - _second->fY), 2.f));
	
	if (Distance <= (_first->fCX + _second->fCX) / 2)
	{
		return LEFT;
	}
	else
		return DIR_NONE;
}

DIR CCollisionManager::Collision_Enter_ES(INFO* _first, RECT* _second)
{
	float DistanceList[4] = {};
	bool Check = false;
	DistanceList[0] = sqrt(pow((_first->fX - _second->left), 2.f) + pow((_first->fY - _second->top), 2.f));
	DistanceList[1] = sqrt(pow((_first->fX - _second->left), 2.f) + pow((_first->fY - _second->bottom), 2.f));
	DistanceList[2] = sqrt(pow((_first->fX - _second->right), 2.f) + pow((_first->fY - _second->top), 2.f));
	DistanceList[3] = sqrt(pow((_first->fX - _second->right), 2.f) + pow((_first->fY - _second->bottom), 2.f));

	for (const auto i : DistanceList)
	{
		if (i <= _first->fCX / 2)
		{
			Check = true;
			break;
		}
	}
	if (_first->fX >= _second->left && _first->fX <= _second->right)
	{
		if (_first->fY >= _second->top && _first->fY <= _second->bottom)
			Check = true;
	}
	
	if (Check)
	{
		if (_first->fX <= _second->left)
		{
			return RIGHT;
		}
		else if (_first->fX >= _second->right)
		{
				return LEFT;
		}
		else if (_first->fY >= _second->top)
		{
			return BOTTOM;
		}
		else if (_first->fX >= _second->bottom)
		{
			return TOP;
		}
	}
	else
		return DIR_NONE;
}

DIR CCollisionManager::Collision_Enter_SE(RECT* _second, INFO* _first)
{
	float DistanceList[4] = {};
	bool Check = false;
	DistanceList[0] = sqrt(pow((_first->fX - _second->left), 2.f) + pow((_first->fY - _second->top), 2.f));
	DistanceList[1] = sqrt(pow((_first->fX - _second->left), 2.f) + pow((_first->fY - _second->bottom), 2.f));
	DistanceList[2] = sqrt(pow((_first->fX - _second->right), 2.f) + pow((_first->fY - _second->top), 2.f));
	DistanceList[3] = sqrt(pow((_first->fX - _second->right), 2.f) + pow((_first->fY - _second->bottom), 2.f));

	for (const auto i : DistanceList)
	{
		if (i <= _first->fCX / 2)
		{
			Check = true;
			break;
		}
	}
	if (_first->fX >= _second->left && _first->fX <= _second->right)
	{
		if (_first->fY >= _second->top && _first->fY <= _second->bottom)
			Check = true;
	}

	if (Check)
	{
		if (_first->fX <= _second->left)
		{
			return LEFT;
		}
		else if (_first->fX >= _second->right)
		{
			return RIGHT;
		}
		else if (_first->fY >= _second->top)
		{
			return TOP;
		}
		else if (_first->fX >= _second->bottom)
		{
			return BOTTOM;
		}
	}
	else
		return DIR_NONE;
}


