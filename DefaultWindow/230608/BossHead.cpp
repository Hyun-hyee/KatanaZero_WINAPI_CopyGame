#include "stdafx.h"
#include "BossHead.h"
#include "BmpMgr.h"
#include "LineMgr.h"

CBossHead::CBossHead()
{
	m_Type = ENEMY;
	m_Collider_type = RECTANGLE;
}

CBossHead::~CBossHead()
{
	Release();
}

void CBossHead::Initialize()
{
	InitImage(); 
	m_State = BOSS_HEADFLY;
	m_fSpeed = 2.f;
	m_tInfo.fCX = 28 * SMALL;
	m_tInfo.fCY = 28 * SMALL;

	m_bJump = true;
	m_fSpeed_Vertical = 4.f;
}

void CBossHead::Update()
{
	StateUpdate();
	
	if (g_SlowMotion)
	{
		m_fSpeed = 0.5f;
	}
	
	Jump();

	__super::Update_Rect();

	CObj::Move_Frame();
}

void CBossHead::LateUpdate(void)
{
}

void CBossHead::Render(HDC hdc)
{
	CObj::FrameRender(hdc);

	CObj::CollideRender(hdc);
}

void CBossHead::Release(void)
{
}

void CBossHead::InitImage()
{

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headfly_8x2.bmp", L"BOSS_HEADFLY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headground_8x2.bmp", L"BOSS_HEADGROUND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/boss/boss_headground_stop_1x2.png", L"BOSS_HEADGROUND_STOP");

	FRAME TempFrame;
	TempFrame.AnimKey = L"BOSS_HEADFLY";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 40;
	TempFrame.iFrameSizeY = 42;
	m_FrameMap.insert({ BOSS_HEADFLY, TempFrame });

	TempFrame.AnimKey = L"BOSS_HEADGROUND";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 90;
	TempFrame.iFrameSizeY = 28;
	m_FrameMap.insert({ BOSS_HEADGROUND, TempFrame });

	TempFrame.AnimKey = L"BOSS_HEADGROUND_STOP";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 90;
	TempFrame.iFrameSizeY = 28;
	m_FrameMap.insert({ BOSS_HEADGROUND_STOP, TempFrame });
}

void CBossHead::StateUpdate()
{

	if (m_PrevState != m_State)
	{
		m_FrameMap[m_State].iFrameStart = 0;
		m_FrameMap[m_State].dwTime = GetTickCount64();

		m_PrevState = m_State;
	}


	switch (m_State)
	{
	case BOSS_HEADFLY:
		if (m_bJump)
		{
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 3.f;
				else
					m_fFrontAngle = PI;
			}
			else if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[LEFT])
					m_tInfo.fX -= 3.f;
				else
					m_fFrontAngle = 0;
			}
		}
		else
		{
			Set_State(BOSS_HEADGROUND);
			m_tInfo.fX += cos(m_fFrontAngle) * 30.f;
		}
			
		break;

	case BOSS_HEADGROUND:
		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			Set_State(BOSS_HEADGROUND_STOP);

		if (m_fFrontAngle == 0)
		{
			if (m_DirCheck[RIGHT])
				m_fFrontAngle = PI;
		}
		else if (m_fFrontAngle == PI)
		{
			if (m_DirCheck[LEFT])
				m_fFrontAngle = 0;
		}

		break;

	case BOSS_HEADGROUND_STOP:
		break;

	}

	if (m_fFrontAngle == 0)
	{
		m_FrameMap[m_State].iMotion = 0;
	}
	else if (m_fFrontAngle == PI)
	{
		m_FrameMap[m_State].iMotion = 1;
	}
}

void CBossHead::Jump()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);


	if (0.f < fY - m_tInfo.fY)
		m_bJump = true;

	if (m_tInfo.fY - m_tInfo.fCY * 0.5f >= 550.f)
	{
		m_bJump = false;
		m_fSpeed_Vertical = 0.f;
		m_tInfo.fY = 550.f + m_tInfo.fCY * 0.5f;
	}
	else
		m_bJump = true;

	if (m_bJump)
	{

		/*if (bLineCol && (fY < m_tInfo.fY - m_fSpeed_Vertical) && m_fSpeed_Vertical < 0.f)
		{
			m_bJump = false;
			m_tInfo.fY = fY;
			m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
		}
		else*/
		{
			if (g_SlowMotion || g_BossDead)
			{
				if (g_SlowJumpTime + 80 < GetTickCount64())
				{
					if (m_State != GRAB_WALL)
					{
						m_tInfo.fY -= m_fSpeed_Vertical;

						if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
						else
							m_fSpeed_Vertical -= (0.034f * G) * 1.2f;
					}

					g_SlowJumpTime = GetTickCount64();
				}

			}
			else
			{

				m_tInfo.fY -= m_fSpeed_Vertical;

				if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
					m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
				else
					m_fSpeed_Vertical -= (0.034f * G) * 1.2f;


			}

		}
	}
	/*else if (bLineCol)
	{
		m_tInfo.fY = fY;
		m_fSpeed_Vertical = 0.f;
	}*/
}




int CBossHead::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		m_DirCheck[_dir] = true;
		if (_dir == TOP)
			m_fSpeed_Vertical *= (-0.2f);
	}
	return OBJ_NOEVENT;
}

int CBossHead::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
	}

	return OBJ_NOEVENT;
}

int CBossHead::OnCollision(CObj* _target, DIR _dir)
{
	return OBJ_NOEVENT;
}

