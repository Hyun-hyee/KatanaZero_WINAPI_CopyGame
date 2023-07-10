#include "stdafx.h"
#include "GrabWall.h"

CGrabWall::CGrabWall()
{
	m_Type = GRABWALL;
	m_Collider_type = RECTANGLE;
}

CGrabWall::~CGrabWall()
{
}

void CGrabWall::Initialize(void)
{
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
}

void CGrabWall::Update(void)
{
}

void CGrabWall::LateUpdate(void)
{
}

void CGrabWall::Render(HDC hDC)
{
	if (g_CollideCheck)
		CObj::CollideRender(hDC);
}

void CGrabWall::Release(void)
{
}

void CGrabWall::InitImage()
{
}

int CGrabWall::InCollision(CObj* _target, DIR _dir)
{

	return 0;
}

int CGrabWall::OutCollision(CObj* _target)
{

	return 0;
}

int CGrabWall::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
