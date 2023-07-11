#include "stdafx.h"
#include "BombEffect.h"
#include "BmpMgr.h"

CBombEffect::CBombEffect()
{
	m_Type = EFFECT;
}

CBombEffect::~CBombEffect()
{
	Release();
}

void CBombEffect::Initialize()
{
	InitImage();
	m_fAttackAngle = 0.f;
	m_EffectTime = GetTickCount64();
	m_Owner = nullptr;
	m_DistanceX = 0.f;
	m_DistanceY = 0.f;
	m_State = BOMB_EXPLOSION;
}

void CBombEffect::Update()
{
	CObj::Update_Rect();

	if (m_FrameMap[m_State].iFrameEnd != 0)
	{
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(DEAD);
		CObj::Move_Frame();
	}

}

void CBombEffect::LateUpdate(void)
{
}

void CBombEffect::Render(HDC hdc)
{
	if (m_fFrontAngle == 0)
	{
		m_FrameMap[m_State].iMotion = 0;
	}
	else
	{
		m_FrameMap[m_State].iMotion = 1;
	}
	if (!g_BossDead)
	{
		if(m_State == BOMB_EXPLOSION)
			CObj::FrameRender_OriginSize(hdc);
		else
			CObj::FrameRender(hdc);
	}

	if (g_CollideCheck)
		CObj::CollideRender(hdc);
}

void CBombEffect::Release(void)
{
}

void CBombEffect::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/Explosion2.png", L"BOMB_EXPLOSION");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/effect/effect_gunspark_4x2.bmp", L"GUN_FIRE");


	FRAME TempFrame;
	TempFrame.AnimKey = L"BOMB_EXPLOSION";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 200;
	TempFrame.iFrameSizeY = 172;
	m_FrameMap.insert({ BOMB_EXPLOSION, TempFrame });

	TempFrame.AnimKey = L"GUN_FIRE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 3;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 98;
	TempFrame.iFrameSizeY = 78;
	m_FrameMap.insert({ GUN_FIRE, TempFrame });


}

int CBombEffect::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CBombEffect::OutCollision(CObj* _target)
{
	return 0;
}

int CBombEffect::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}


