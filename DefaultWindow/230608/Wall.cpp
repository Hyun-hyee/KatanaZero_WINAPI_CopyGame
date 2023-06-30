#include "stdafx.h"
#include "Wall.h"
#include "SceneManager.h"

CWall::CWall()
{
	m_Type = WALL;
	m_Collider_type = RECTANGLE;
}

CWall::~CWall()
{
	Release();
}

void CWall::Initialize(void)
{
}

void CWall::Update(void)
{
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
}

void CWall::LateUpdate(void)
{
}

void CWall::Render(HDC hDC)
{
	fPOINT cameraPos = CSceneManager::Get_Instance()->GetCameraPos();
	
	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);
	
	HPEN myPen = CreatePen(PS_SOLID, 0, RGB(255, 0, 255));
	HPEN oldPen = (HPEN)SelectObject(hDC, myPen);

	Rectangle(hDC, m_tRect.left - ((float)cameraPos.x - WINCX / 2),
		m_tRect.top - ((float)cameraPos.y - WINCY / 2),
		m_tRect.right - ((float)cameraPos.x - WINCX / 2),
		m_tRect.bottom - ((float)cameraPos.y - WINCY / 2));

	SelectObject(hDC, oldBrush);
	DeleteObject(myBrush);

	SelectObject(hDC, oldPen);
	DeleteObject(myPen);
}

void CWall::Release(void)
{
}

void CWall::InitImage()
{
}

int CWall::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CWall::OutCollision(CObj* _target)
{
	return 0;
}
