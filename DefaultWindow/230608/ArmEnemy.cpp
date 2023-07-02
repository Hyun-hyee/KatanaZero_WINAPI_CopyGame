#include "stdafx.h"
#include "ArmEnemy.h"
#include "ObjFactory.h"
#include "BmpMgr.h"
#include "LineMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"

CArmEnemy::CArmEnemy()
{
	m_Type = ENEMY;
	m_Collider_type = RECTANGLE;
}

CArmEnemy::~CArmEnemy()
{
	Release();
}

void CArmEnemy::Initialize(void)
{
	InitImage();
	m_State = IDLE;
	m_fSpeed = 3.f;
	m_CheckCWidth = 70.f;
	m_bFollow = false;

	m_Target = CObjMgr::Get_Instance()->Get_Player();
}

void CArmEnemy::Update(void)
{
	StateUpdate();

	Jump();
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
	Update_CheckCollide();
	
	CObj::Move_Frame();
}

void CArmEnemy::LateUpdate(void)
{
}

void CArmEnemy::Render(HDC hDC)
{
	CObj::FrameRender(hDC);

	CObj::CollideRender(hDC);
	CObj::CollideRender(hDC,m_CheckCollide);
}

void CArmEnemy::Release(void)
{
}

void CArmEnemy::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_grunt_idle_8x2.bmp", L"ArmEnemy_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_grunt_walk_10x2.bmp", L"ArmEnemy_WALK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_grunt_run_10x2.bmp", L"ArmEnemy_RUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_grunt_attack_7x2.bmp", L"ArmEnemy_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_grunt_hurt_15x2.bmp", L"ArmEnemy_HURT");

	FRAME TempFrame;
	TempFrame.AnimKey = L"ArmEnemy_IDLE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 60;
	TempFrame.iFrameSizeY = 72;
	m_FrameMap.insert({ IDLE, TempFrame });

	TempFrame.AnimKey = L"ArmEnemy_WALK";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 64;
	TempFrame.iFrameSizeY = 80;
	m_FrameMap.insert({ WALK, TempFrame });

	TempFrame.AnimKey = L"ArmEnemy_RUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 72;
	TempFrame.iFrameSizeY = 78;
	m_FrameMap.insert({ RUN, TempFrame });
	 
	TempFrame.AnimKey = L"ArmEnemy_ATTACK";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 6;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 88;
	TempFrame.iFrameSizeY = 84;
	m_FrameMap.insert({ ATTACK, TempFrame });

	TempFrame.AnimKey = L"ArmEnemy_HURT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 14;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 112;
	TempFrame.iFrameSizeY = 82;
	m_FrameMap.insert({ HURT, TempFrame });
}

void CArmEnemy::Attack()
{
}

void CArmEnemy::StateUpdate()
{
	float fTargetX = m_Target->Get_Info()->fX;

	if (m_fFrontAngle == 0)
	{
		m_FrameMap[m_State].iMotion = 0;
	}
	else if (m_fFrontAngle == PI)
	{
		m_FrameMap[m_State].iMotion = 1;
	}

	if (m_PrevState != m_State)
	{
		m_FrameMap[m_State].iFrameStart = 0;
		m_FrameMap[m_State].dwTime = GetTickCount64();

		m_PrevState = m_State;
	}

	switch (m_State)
	{
	case IDLE:
		
		if (m_bFollow)
		{
			if (m_tInfo.fX + 30.f < fTargetX)
			{
				m_State = RUN;
			}
			else if (m_tInfo.fX - 30.f > fTargetX)
			{
				m_State = RUN;
			}
		}
		else if (CheckTargetFront())
		{
			m_bFollow = true;
			m_State = RUN;
		}

		break;

	case WALK:
		break;

	case RUN:
		if (m_bFollow)
		{
			if (m_tInfo.fX + 30.f < fTargetX)
			{
				m_fFrontAngle = 0;
 				m_tInfo.fX += cos(m_fFrontAngle) * (m_fSpeed);
			}
			else if (m_tInfo.fX - 30.f > fTargetX)
			{
				m_fFrontAngle = PI;
				m_tInfo.fX += cos(m_fFrontAngle) * (m_fSpeed);
			}
			else if (m_tInfo.fX - 30.f <= fTargetX && m_tInfo.fX + 30.f >= fTargetX)
 				m_State = IDLE;
		}

		if (!CheckTargetFront())
		{
			m_State = IDLE;
			m_bFollow = false;
		}
			
		break;

	case ATTACK:
		break;

	case HURT:
		break;
	}

}

void CArmEnemy::Jump()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (0.f < fY - m_tInfo.fY)
		m_bJump = true;


	if (m_bJump)
	{

		if (bLineCol && (fY < m_tInfo.fY - m_fSpeed_Vertical) && m_fSpeed_Vertical < 0.f)
		{
			m_bJump = false;
			m_tInfo.fY = fY;
			m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
		}
		else
		{
			if (g_SlowMotion)
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
					else
					{
						if (m_fSpeed_Vertical < -5.f)
							m_fSpeed_Vertical = -5.f;
						m_tInfo.fY -= m_fSpeed_Vertical;

						if (-1.f < m_fSpeed_Vertical && 1.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 0.2f;
						else if (-4.f < m_fSpeed_Vertical && 4.f > m_fSpeed_Vertical)
							m_fSpeed_Vertical -= (0.034f * G) * 0.4f;
						else
							m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
					}
					g_SlowJumpTime = GetTickCount64();
				}

			}
			else
			{
				if (m_State != GRAB_WALL)
				{
					m_tInfo.fY -= m_fSpeed_Vertical;

					if (-2.f < m_fSpeed_Vertical && 2.f > m_fSpeed_Vertical)
						m_fSpeed_Vertical -= (0.034f * G) * 0.6f;
					else
						m_fSpeed_Vertical -= (0.034f * G) * 1.2f;
				}
				else
				{
					if (m_fSpeed_Vertical < -5.f)
						m_fSpeed_Vertical = -5.f;
					m_tInfo.fY -= m_fSpeed_Vertical;

					if (-1.f < m_fSpeed_Vertical && 1.f > m_fSpeed_Vertical)
						m_fSpeed_Vertical -= (0.034f * G) * 0.05f;
					else
						m_fSpeed_Vertical -= (0.034f * G) * 0.8f;
				}
			}

		}
	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
		m_fSpeed_Vertical = 0.f;
	}


}

void CArmEnemy::Update_CheckCollide()
{
	if (m_fFrontAngle == PI)
	{
		m_CheckCollide.left = m_FrontCollide.left - m_CheckCWidth;
		m_CheckCollide.right = m_FrontCollide.right;
	}
	else
	{
		m_CheckCollide.right = m_FrontCollide.right + m_CheckCWidth;
		m_CheckCollide.left = m_FrontCollide.left;
	}
	m_CheckCollide.top = m_FrontCollide.top - 30.f;
	m_CheckCollide.bottom = m_FrontCollide.bottom;
}

bool CArmEnemy::CheckTargetFront()
{
	if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_CheckCollide, m_Target->Get_Collide()))
		return true;
	else
		return false;
}

int CArmEnemy::InCollision(CObj* _target, DIR _dir)
{
	
	return OBJ_NOEVENT;
}

int CArmEnemy::OutCollision(CObj* _target)
{

	return OBJ_NOEVENT;
}

int CArmEnemy::OnCollision(CObj* _target)
{

	return OBJ_NOEVENT;
}


