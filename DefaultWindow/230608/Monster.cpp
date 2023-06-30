#include "stdafx.h"
#include "Monster.h"
#include "Bullet.h"
#include "ObjFactory.h"

CMonster::CMonster()
{
	m_Type = ENEMY;
	m_Collider_type = RECTANGLE;
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize(void)
{
	m_fSpeed = 3.f;
}

void CMonster::Update(void)
{
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
}

void CMonster::LateUpdate(void)
{
}

void CMonster::Render(HDC hDC)
{
	
}

void CMonster::Release(void)
{
}

void CMonster::InitImage()
{
}

void CMonster::Attack()
{
}

int CMonster::OnCollision(CObj* _target, DIR _dir)
{
	
	return OBJ_NOEVENT;
}

int CMonster::OutCollision(CObj* _target)
{
	return 0;
}


