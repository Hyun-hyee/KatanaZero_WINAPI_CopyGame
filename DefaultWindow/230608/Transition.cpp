#include "stdafx.h"
#include "Transition.h"
#include "BmpMgr.h"

CTransition::CTransition()
{
}

CTransition::~CTransition()
{
}

void CTransition::Initialize()
{/*
	m_tInfo.fCX = 32;
	m_tInfo.fCY = 768;
	m_State = TRANSITIONSTART;
	InitImage();*/
}

void CTransition::Update()
{/*
	if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		Set_State(DEAD);
	CObj::Move_Frame();*/
}

void CTransition::LateUpdate(void)
{
}

void CTransition::Render(HDC hdc)
{
	CObj::FrameRender(hdc);
}

void CTransition::Release(void)
{
}

void CTransition::InitImage()
{
	//Transtion
	/*CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/Transition/TransitionALL.png", L"TRANSITIONSTART");

	FRAME Temp;
	Temp.AnimKey = L"TRANSITIONSTART";
	Temp.iFrameStart = 0;
	Temp.iFrameEnd = 76;
	Temp.iMotion = 0;
	Temp.dwSpeed = 60;
	Temp.dwTime = GetTickCount64();
	Temp.iFrameSizeX = 32;
	Temp.iFrameSizeY = 768;
	m_FrameMap.insert({ TRANSITIONSTART, Temp });*/
}

int CTransition::InCollision(CObj* _target, DIR _dir)
{
	return 0;
}

int CTransition::OutCollision(CObj* _target)
{
	return 0;
}

int CTransition::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}
