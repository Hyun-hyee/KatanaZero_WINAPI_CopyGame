#include "stdafx.h"
#include "BloodEffect.h"
#include "BmpMgr.h"

CBloodEffect::CBloodEffect()
{
	m_Type = EFFECT;
}

CBloodEffect::~CBloodEffect()
{
	Release();
}

void CBloodEffect::Initialize()
{
	InitImage();
	m_fAttackAngle = 0.f;
	m_EffectTime = GetTickCount64();
	m_Owner = nullptr;
	m_DistanceX = 0.f;
	m_DistanceY = 0.f;
}

void CBloodEffect::Update()
{
	if (m_Owner != nullptr)
	{
		INFO* OwnerInfo = m_Owner->Get_Info();
		m_tInfo.fX = OwnerInfo->fX + cos(m_fFrontAngle) * m_DistanceX;
		m_tInfo.fY = OwnerInfo->fY + m_DistanceY;
	}

	CObj::Update_Rect();

	if (m_FrameMap[m_State].iFrameEnd != 0)
	{
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(DEAD);
		CObj::Move_Frame();
	}

}

void CBloodEffect::LateUpdate(void)
{
}

void CBloodEffect::Render(HDC hdc)
{
	if (!g_BossDead)
	{
		if (m_State == BLOOD_EFFECT_MOVE)
			CObj::FrameRender(hdc);
		if (m_State == BLOOD_EFFECT_ONE)
			CObj::RotateFrameRender(hdc, 360.f - m_fAttackAngle * (180.f / PI), 5.f, 5.f);
		else
			CObj::RotateFrameRender(hdc, 360.f - m_fAttackAngle * (180.f / PI), 2.f, 2.f);

		CObj::CollideRender(hdc);
	}
}

void CBloodEffect::Release(void)
{
}

void CBloodEffect::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/0.png", L"BLOOD_EFFECT_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/1.png", L"BLOOD_EFFECT_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/2.png", L"BLOOD_EFFECT_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/3.png", L"BLOOD_EFFECT_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/4.png", L"BLOOD_EFFECT_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/5.png", L"BLOOD_EFFECT_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/6.png", L"BLOOD_EFFECT_6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/7.png", L"BLOOD_EFFECT_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/8.png", L"BLOOD_EFFECT_8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/bloodone.png", L"BLOOD_EFFECT_ONE");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/blood/bloodMove_7x1.png", L"BLOOD_EFFECT_MOVE");


	FRAME TempFrame;
	TempFrame.AnimKey = L"BLOOD_EFFECT_0";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 145;
	TempFrame.iFrameSizeY = 29;
	m_FrameMap.insert({ BLOOD_EFFECT_0, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_1";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 96;
	TempFrame.iFrameSizeY = 27;
	m_FrameMap.insert({ BLOOD_EFFECT_1, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_2";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 294;
	TempFrame.iFrameSizeY = 32;
	m_FrameMap.insert({ BLOOD_EFFECT_2, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_3";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 88;
	TempFrame.iFrameSizeY = 48;
	m_FrameMap.insert({ BLOOD_EFFECT_3, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_4";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 151;
	TempFrame.iFrameSizeY = 77;
	m_FrameMap.insert({ BLOOD_EFFECT_4, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_5";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 189;
	TempFrame.iFrameSizeY = 95;
	m_FrameMap.insert({ BLOOD_EFFECT_5, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_6";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 57;
	TempFrame.iFrameSizeY = 41;
	m_FrameMap.insert({ BLOOD_EFFECT_6, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_7";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 83;
	TempFrame.iFrameSizeY = 45;
	m_FrameMap.insert({ BLOOD_EFFECT_7, TempFrame });

	TempFrame.AnimKey = L"BLOOD_EFFECT_8";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 164;
	TempFrame.iFrameSizeY = 80;
	m_FrameMap.insert({ BLOOD_EFFECT_8, TempFrame });


	TempFrame.AnimKey = L"BLOOD_EFFECT_ONE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 16;
	TempFrame.iFrameSizeY = 16;
	m_FrameMap.insert({ BLOOD_EFFECT_ONE, TempFrame });


	TempFrame.AnimKey = L"BLOOD_EFFECT_MOVE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 32;
	TempFrame.iFrameSizeY = 31;
	m_FrameMap.insert({ BLOOD_EFFECT_MOVE, TempFrame });


}

int CBloodEffect::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CBloodEffect::OutCollision(CObj* _target)
{
	return 0;
}

int CBloodEffect::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}

void CBloodEffect::Set_RandomState()
{
	int randnum = rand()%9;

	switch (randnum)
	{
	case 0:
		m_State = BLOOD_EFFECT_0;
		break;

	case 1:
		m_State = BLOOD_EFFECT_1;
		break;

	case 2:
		m_State = BLOOD_EFFECT_2;
		break;

	case 3:
		m_State = BLOOD_EFFECT_3;
		break;

	case 4:
		m_State = BLOOD_EFFECT_4;
		break;

	case 5:
		m_State = BLOOD_EFFECT_5;
		break;

	case 6:
		m_State = BLOOD_EFFECT_6;
		break;

	case 7:
		m_State = BLOOD_EFFECT_7;
		break;

	case 8:
		m_State = BLOOD_EFFECT_8;
		break;
	}



}
