#include "stdafx.h"
#include "PlayerEffect.h"
#include "BmpMgr.h"

CPlayerEffect::CPlayerEffect()
{
	m_Type = EFFECT;
}

CPlayerEffect::~CPlayerEffect()
{
	Release();
}

void CPlayerEffect::Initialize()
{
	InitImage();
	m_fAttackAngle = 0.f;
	m_EffectTime = GetTickCount64();
	m_Owner = nullptr;
	m_DistanceX = 0.f;
	m_DistanceY = 0.f;
}

void CPlayerEffect::Update()
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
	}
	else
	{
		if (m_EffectTime + 200 < GetTickCount64())
			Set_State(DEAD);
		if (m_State == PLAYER_EFFECT_HIT)
		{
			m_tInfo.fX += cos(m_fAttackAngle) * 10.f;
			m_tInfo.fY -= sin(m_fAttackAngle) * 10.f;
		}
	}

	CObj::Move_Frame();
}

void CPlayerEffect::LateUpdate(void)
{
}

void CPlayerEffect::Render(HDC hdc)
{
	if(m_State == PLAYER_EFFECT_HIT)
		CObj::RotateFrameRender(hdc, 360.f -  m_fAttackAngle * (180.f / PI),5.f,1.f);
	else if (m_State == PLAYER_EFFECT_RUNDUST)
 		CObj::RotateFrameRender(hdc, 360.f - m_fAttackAngle * (180.f / PI), 2.f, 2.f);
	else
		CObj::RotateFrameRender(hdc, 360.f - m_fAttackAngle * (180.f / PI), 1.f, 1.f);


	//	CObj::CollideRender(hdc);
}

void CPlayerEffect::Release(void)
{
}

void CPlayerEffect::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/effect_hit_lazer.bmp", L"PLAYER_EFFECT_HIT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/shadow/player_slash_basic_6x2.bmp", L"PLAYER_EFFECT_SLASH");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/effect/player_dust_7x1.bmp", L"PLAYER_EFFECT_RUNDUST"); 
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/effect/player_jumpdust_5x1.bmp", L"PLAER_EFFECT_JUMPDUST");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/effect/player_landdust_7x1.png", L"PLAYER_EFFECT_LANDDUST");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/effect_reflect_6x2.bmp", L"PLAYER_EFFECT_REFLECT");


	FRAME TempFrame;
	TempFrame.AnimKey = L"PLAYER_EFFECT_HIT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 900;
	TempFrame.iFrameSizeY = 4;
	m_FrameMap.insert({ PLAYER_EFFECT_HIT, TempFrame });

	TempFrame.AnimKey = L"PLAYER_EFFECT_SLASH";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 5;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 210;
	TempFrame.iFrameSizeY = 60;
	m_FrameMap.insert({ PLAYER_EFFECT_SLASH, TempFrame });

	TempFrame.AnimKey = L"PLAYER_EFFECT_RUNDUST";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 34;
	TempFrame.iFrameSizeY = 28;
	m_FrameMap.insert({ PLAYER_EFFECT_RUNDUST, TempFrame });

	TempFrame.AnimKey = L"PLAER_EFFECT_JUMPDUST";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 4;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 42;
	TempFrame.iFrameSizeY = 80;
	m_FrameMap.insert({ PLAER_EFFECT_JUMPDUST, TempFrame });

	TempFrame.AnimKey = L"PLAYER_EFFECT_LANDDUST";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 100;
	TempFrame.iFrameSizeY = 28;
	m_FrameMap.insert({ PLAYER_EFFECT_LANDDUST, TempFrame });

	TempFrame.AnimKey = L"PLAYER_EFFECT_REFLECT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 5;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 148;
	TempFrame.iFrameSizeY = 140;
	m_FrameMap.insert({ PLAYER_EFFECT_REFLECT, TempFrame });

}

int CPlayerEffect::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CPlayerEffect::OutCollision(CObj* _target)
{
	return 0;
}

int CPlayerEffect::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
