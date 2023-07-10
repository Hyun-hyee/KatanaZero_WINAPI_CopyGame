#include "stdafx.h"
#include "Fan.h"
#include "BmpMgr.h"

CFan::CFan()
{
	m_Type = FAN;
	m_Collider_type = RECTANGLE;
}

CFan::~CFan()
{
	Release();
}

void CFan::Initialize()
{
	InitImage();
	m_State = FAN_ON;
	m_fFrontAngle = 0;
	m_tInfo.fCX = 53;
	m_tInfo.fCY = 140;
	m_AttackOn = true;
}

void CFan::Update()
{
	if (m_FrameMap[m_State].iFrameStart >= 10 && m_FrameMap[m_State].iFrameStart < 20)
		m_AttackOn = false;
	else
		m_AttackOn = true;

	__super::Update_Rect();
	CObj::Move_Frame();
}

void CFan::LateUpdate(void)
{
}

void CFan::Render(HDC hDC)
{
	CObj::FrameRender_OriginSize(hDC);
	
	if (g_CollideCheck)
		CObj::CollideRender(hDC);
}

void CFan::Release(void)
{
}

void CFan::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/spr_fanblade/fan_32x1_resize.png", L"FAN_ON");

	FRAME TempFrame;
	TempFrame.AnimKey = L"FAN_ON";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 31;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 53;
	TempFrame.iFrameSizeY = 140;
	m_FrameMap.insert({ FAN_ON, TempFrame });
}

int CFan::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CFan::OutCollision(CObj* _target)
{
	return 0;
}

int CFan::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
