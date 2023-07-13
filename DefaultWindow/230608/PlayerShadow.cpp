#include "stdafx.h"
#include "PlayerShadow.h"
#include "BmpMgr.h"

CPlayerShadow::CPlayerShadow()
{
	m_Type = EFFECT;
}

CPlayerShadow::~CPlayerShadow()
{
	Release();
}

void CPlayerShadow::Initialize()
{
	InitImage();
	m_fAttackAngle = 0.f;
	m_EffectTime = GetTickCount64();
	m_Owner = nullptr;
	m_DistanceX = 0.f;
	m_DistanceY = 0.f;
}

void CPlayerShadow::Update()
{	
	//if (m_FrameMap[m_State].iFrameEnd != 0)
	{
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(DEAD);
	}


	CObj::Update_Rect();
	CObj::Move_Frame();
}

void CPlayerShadow::LateUpdate(void)
{
}

void CPlayerShadow::Render(HDC hdc)
{
	if (m_Owner->Get_FrontAngle() == 0)
	{
		m_fFrontAngle = 0;
		m_FrameMap[m_State].iMotion = 0;
	}
	else
	{
		m_fFrontAngle = PI;
		m_FrameMap[m_State].iMotion = 1;
	}
	if(!g_BossDead)
		CObj::FrameRender(hdc);
}

void CPlayerShadow::Release(void)
{
}

void CPlayerShadow::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/shadow/player_attack_shadow_7x2.png", L"PLAER_EFFECT_ATTACKSHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/shadow/player_flip_shadow_11x2.png", L"PLAER_EFFECT_FLIPSHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/shadow/player_fall_shadow_4x2.png", L"PLAER_EFFECT_FALLSHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/shadow/player_roll_shadow_7x2.png", L"PLAER_EFFECT_ROLLSHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/player/shadow/player_run_shadow_10x2.png", L"PLAER_EFFECT_RUNSHADOW");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_walljump_shadow_7x2.png", L"BOSS_EFFECT_WALLJUMPSHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_jump_shadow_1x2.png", L"BOSS_EFFECT_JUMPSHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_dash_shadow_1x2.png", L"BOSS_EFFECT_DASHSHADOW");


	FRAME TempFrame;
	
	////////SHADOW////////
	TempFrame.AnimKey = L"PLAER_EFFECT_ATTACKSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 120;
	TempFrame.iFrameSizeY = 82;
	m_FrameMap.insert({	ATTACK, TempFrame });

	TempFrame.AnimKey = L"PLAER_EFFECT_FLIPSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 10;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 96;
	TempFrame.iFrameSizeY = 88;
	m_FrameMap.insert({ JUMP_WALL, TempFrame });

	TempFrame.AnimKey = L"PLAER_EFFECT_FALLSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 82;
	TempFrame.iFrameSizeY = 96;
	m_FrameMap.insert({ FALL, TempFrame });

	TempFrame.AnimKey = L"PLAER_EFFECT_ROLLSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 94;
	TempFrame.iFrameSizeY = 64;
	m_FrameMap.insert({ ROLL, TempFrame });

	TempFrame.AnimKey = L"PLAER_EFFECT_RUNSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 88;
	TempFrame.iFrameSizeY = 64;
	m_FrameMap.insert({ RUN, TempFrame });

	//********************************************//
	TempFrame.AnimKey = L"BOSS_EFFECT_WALLJUMPSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 108;
	TempFrame.iFrameSizeY = 124;
	m_FrameMap.insert({ BOSS_WALLJUMP, TempFrame });

	TempFrame.AnimKey = L"BOSS_EFFECT_JUMPSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 54;
	TempFrame.iFrameSizeY = 88;
	m_FrameMap.insert({ BOSS_JUMP, TempFrame });

	TempFrame.AnimKey = L"BOSS_EFFECT_DASHSHADOW";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 0;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 102;
	TempFrame.iFrameSizeY = 50;
	m_FrameMap.insert({ BOSS_DASH, TempFrame });

}

int CPlayerShadow::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CPlayerShadow::OutCollision(CObj* _target)
{
	return 0;
}

int CPlayerShadow::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
