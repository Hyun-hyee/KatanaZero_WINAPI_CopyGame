#include "stdafx.h"
#include "UIObject.h"
#include "BmpMgr.h"

CUIObject::CUIObject()
{
	m_Type = FAN;
	m_Collider_type = RECTANGLE;
}

CUIObject::~CUIObject()
{
	Release();
}

void CUIObject::Initialize()
{
	InitImage();
	m_State = UI_GO;
	m_fFrontAngle = 0;
	m_tInfo.fCX = 10;
	m_tInfo.fCY = 10;
	m_FirstX = 0.f;
	m_FirstY = 0.f;
}

void CUIObject::Update()
{
	if (m_FirstY != 0.f)
	{
		if (m_tInfo.fY <= m_FirstY + 5.f)
			m_tInfo.fY += 0.1f;
		else
			m_tInfo.fY = m_FirstY;
	}
	else
	{
		if (m_tInfo.fX <= m_FirstX + 5.f)
			m_tInfo.fX += 0.1f;
		else
			m_tInfo.fX = m_FirstX;
	}

	__super::Update_Rect();
	CObj::Move_Frame();
}

void CUIObject::LateUpdate(void)
{
}

void CUIObject::Render(HDC hDC)
{
	CObj::FrameRender(hDC);

	if (g_CollideCheck)
		CObj::CollideRender(hDC);
}

void CUIObject::Release(void)
{
}

void CUIObject::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/hud_go.bmp", L"UI_GO");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/hud_go_arrow.bmp", L"UI_ARROW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/hud_go_arrow_down.bmp", L"UI_ARROWDOWN");

	FRAME TempFrame;
	TempFrame.AnimKey = L"UI_GO";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 78;
	TempFrame.iFrameSizeY = 40;
	m_FrameMap.insert({ UI_GO, TempFrame });

	
	TempFrame.AnimKey = L"UI_ARROW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 62;
	TempFrame.iFrameSizeY = 38;
	m_FrameMap.insert({ UI_ARROW, TempFrame });


	TempFrame.AnimKey = L"UI_ARROWDOWN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 38;
	TempFrame.iFrameSizeY = 62;
	m_FrameMap.insert({ UI_ARROWDOWN, TempFrame });
}

int CUIObject::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CUIObject::OutCollision(CObj* _target)
{
	return 0;
}

int CUIObject::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
