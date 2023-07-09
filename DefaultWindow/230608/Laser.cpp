#include "stdafx.h"
#include "Laser.h"
#include "stdafx.h"
#include "Bullet.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CLaser::CLaser()
{
	m_Type = BULLET;
	m_Collider_type = RECTANGLE;
}

CLaser::~CLaser()
{
}

void CLaser::Initialize()
{
	m_State = LASER_AIM;
	m_FrameReverse = true;
	m_ChangeTime = 0;
	m_LaserType = LASER_SHORT;
	m_LaserAttack = false;

	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;

	for (int i = 1; i <= 60; ++i)
	{
		float centerX = m_tInfo.fX + 20.f * i * cos(m_fAttackAngle);
		float centerY = m_tInfo.fY + 20.f * i * sin(m_fAttackAngle);
		RECT TempC = { centerX - 10.f, centerY - 10.f, centerX + 10.f, centerY + 10.f };
		m_LaserCollideList.push_back(TempC);
	}

	InitImage();
}

void CLaser::Update()
{
	if (g_SlowMotion || g_BossDead)
	{
		m_fSpeed = 0.8f;
	}
	else
		m_fSpeed = 10.f;
	
	StateUpdate();
	
	if(m_State != LASER_AIM)
		CheckAttackOn();

	Update_Info();
	__super::Update_Rect();
	CObj::UpdateAttackCollide();
	Update_LaserCollide();

	CObj::Move_Frame();
	//m_tInfo.fX += cos(m_fAttackAngle) * m_fSpeed;
	//m_tInfo.fY -= sin(m_fAttackAngle) * m_fSpeed;
}

void CLaser::LateUpdate(void)
{
}

void CLaser::Render(HDC hdc)
{
	CObj::CollideRender(hdc);
	
//	for (auto& iter : m_LaserCollideList)
//		CObj::CollideRender(hdc,iter);

	CObj::RotateFrameRender_Vertical(hdc, m_fAttackAngle * (180.f / PI), 1.f , 0.5f);
}

void CLaser::Release(void)
{
}

void CLaser::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_lazer4.bmp", L"LASER_AIM");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_lazer_ground_1x10.bmp", L"LASER_GROUND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_lazer_1x5.bmp", L"LASER_GROUND_HALF"); //ON->OFF
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_lazer1.bmp", L"LASER_GROUND_STOP"); //ON

	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_lazer_top_10x1.bmp", L"Laser_TOP");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_lazer_top_1x5.bmp", L"Laser_TOP_HALF");
	
	FRAME TempFrame;
	TempFrame.AnimKey = L"LASER_AIM";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 2000;
	TempFrame.iFrameSizeY = 50;
	m_FrameMap.insert({ LASER_AIM, TempFrame });


	TempFrame.AnimKey = L"LASER_GROUND";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 20;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 1500;
	TempFrame.iFrameSizeY = 36;
	m_FrameMap.insert({ LASER_GROUND, TempFrame });

	TempFrame.AnimKey = L"LASER_GROUND_HALF";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 4;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 20;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 2000;
	TempFrame.iFrameSizeY = 50;
	m_FrameMap.insert({ LASER_GROUND_HALF, TempFrame });

	TempFrame.AnimKey = L"LASER_GROUND_STOP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 2000;
	TempFrame.iFrameSizeY = 50;
	m_FrameMap.insert({ LASER_GROUND_STOP, TempFrame });

}

int CLaser::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();


	return OBJ_NOEVENT;
}

int CLaser::OutCollision(CObj* _target)
{
	return 0;
}

int CLaser::OnCollision(CObj* _target, DIR _dir)
{
	return 0;
}

void CLaser::StateUpdate()
{

	if (m_PrevState != m_State)
	{
		m_FrameMap[m_State].iFrameStart = 0;
		m_FrameMap[m_State].dwTime = GetTickCount64();

		m_PrevState = m_State;
	}

	switch (m_State)
	{
	case LASER_AIM :

		if (m_LaserType == LASER_SHORT)
			Set_State(LASER_GROUND);
		else if (m_LaserAttack)
		{
			Set_State(LASER_GROUND_HALF);
			m_ChangeTime = 0;
		}
		else if(m_LaserType == LASER_FOLLOW)
			Update_AttackAngle();

		break;

	case LASER_GROUND :
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(DEAD);

		break;
	
	case LASER_GROUND_HALF :
		if (m_FrameReverse == true && m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			m_FrameMap[m_State].iFrameStart = 0;
			m_FrameMap[m_State].dwTime = GetTickCount64();
			//m_ChangeTime = GetTickCount64();
			Set_State(LASER_GROUND_STOP);
		}
		else if (m_FrameReverse == false && m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd) //레이저 조준 -> 집어넣음
		{
			m_FrameReverse = true;
			//m_ChangeTime = 0;
			Set_State(DEAD);
		}

		break;
	
	case LASER_GROUND_STOP :
		if (!m_LaserAttack)
		{
			m_FrameReverse = false;  
			Set_State(LASER_GROUND_HALF);
		}

		break;
	}
}


void CLaser::Update_AttackAngle()
{
	INFO* PlayerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	

	if (m_fFrontAngle == 0)
	{
		if (PlayerInfo->fX >= m_tInfo.fX)
			m_fAttackAngle = (atan2(PlayerInfo->fY - (m_Axis.y), PlayerInfo->fX - m_Axis.x));

	}
	else
	{
		if (PlayerInfo->fX <= m_tInfo.fX)
			m_fAttackAngle = (atan2(PlayerInfo->fY - (m_Axis.y), PlayerInfo->fX - m_Axis.x));

	}
}

void CLaser::Update_Info()
{
  	m_tInfo.fX = m_Axis.x + m_tInfo.fCX * 0.5f * cos(m_fAttackAngle);
	m_tInfo.fY = m_Axis.y - m_tInfo.fCX * 0.5f * sin(m_fAttackAngle);	
}

void CLaser::Update_LaserCollide()
{
	for (int i = 0; i < 40; ++i)
	{
		float centerX = m_tInfo.fX + 20.f * (i + 1) * cos(m_fAttackAngle);
		float centerY = m_tInfo.fY + 20.f * (i + 1) * sin(m_fAttackAngle);
		m_LaserCollideList[i].left = centerX - 10.f;
		m_LaserCollideList[i].top = centerY - 10.f;
		m_LaserCollideList[i].right = centerX + 10.f;
		m_LaserCollideList[i].bottom = centerY + 10.f;
	}

}

void CLaser::RotateAxis(float _Addx, float _y)
{
	m_Axis.x += _Addx;
	m_Axis.y = _y;
}

void CLaser::CheckAttackOn()
{
	CObj* Target = CObjMgr::Get_Instance()->Get_Player();
	OBJ_STATE TargetState = Target->Get_State();

	for (auto& iter : m_LaserCollideList)
	{
		if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&iter, Target->Get_Collide()))
		{
			if (TargetState != HURTFLY && TargetState != HURTGROUND && TargetState != ROLL)
			{
				Target->Set_State(HURTFLY);
				CSoundMgr::Get_Instance()->PlaySound(L"playerdie.wav", SOUND_EFFECT, SOUND_VOL3);
			}
			break;
		}
	}
}
