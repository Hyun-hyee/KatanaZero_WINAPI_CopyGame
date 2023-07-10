#include "stdafx.h"
#include "LaserObject.h"
#include "BmpMgr.h"

CLaserObject::CLaserObject()
{
	m_Type = LASEROBJECT;
	m_Collider_type = RECTANGLE;
}

CLaserObject::~CLaserObject()
{
	Release();
}

void CLaserObject::Initialize()
{
	InitImage();
	m_State = LASEROBJECT_OFF;
	m_fFrontAngle = 0;
	m_tInfo.fCX = 3;
	m_tInfo.fCY = 213;
	m_AttackOn = true;
}

void CLaserObject::Update()
{
	__super::Update_Rect();
	CObj::Move_Frame();
}

void CLaserObject::LateUpdate(void)
{
}

void CLaserObject::Render(HDC hDC)
{
//	for(int i = 0; i < m_HeightNum; ++i)
		CObj::FrameRender_OriginSize(hDC,0);

	if (g_CollideCheck)
		CObj::CollideRender(hDC);
}

void CLaserObject::Release(void)
{
}

void CLaserObject::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/laser/laser_on_2x1.png", L"LASEROBJECT_ON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/laser/laser_off_4x1.png", L"LASEROBJECT_OFF");

	FRAME TempFrame;
	TempFrame.AnimKey = L"LASEROBJECT_ON";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 1;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 3;
	TempFrame.iFrameSizeY = 213;
	m_FrameMap.insert({ LASEROBJECT_ON, TempFrame });

	TempFrame.AnimKey = L"LASEROBJECT_OFF";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 1;
	TempFrame.iFrameSizeY = 213;
	m_FrameMap.insert({ LASEROBJECT_OFF, TempFrame });
}

int CLaserObject::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == PLAYER || targetType == ENEMY)
	{
		m_AttackOn = true;
		m_State = LASEROBJECT_ON;
	}
	return 0;
}

int CLaserObject::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == PLAYER || targetType == ENEMY)
	{
		m_AttackOn = true;
		m_State = LASEROBJECT_OFF;
	}

	return 0;
}

int CLaserObject::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
