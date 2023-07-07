#include "stdafx.h"
#include "ArmEnemy.h"
#include "ObjFactory.h"
#include "BmpMgr.h"
#include "LineMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "Item.h"
#include "KeyMgr.h"

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
	m_State = WALK;
	m_fSpeed = 1.f;
	m_fAccel = 0.1f;
	m_CheckCWidth = 150.f;
	m_bFollow = false;
	m_HurtOn = false;
	m_BulletHurt = false;
	m_WalkTime = GetTickCount64();

	m_Target = CObjMgr::Get_Instance()->Get_Player();
}

void CArmEnemy::Update(void)
{
	StateUpdate();

	if (g_SlowMotion)
	{
		m_fSpeed = 0.5f;
	}
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
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_grunt_hurtground.png", L"ArmEnemy_HURTGROUND");

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
	TempFrame.dwSpeed = 90;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 112;
	TempFrame.iFrameSizeY = 82;
	m_FrameMap.insert({ HURT, TempFrame });

	TempFrame.AnimKey = L"ArmEnemy_HURTGROUND";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 90;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 112;
	TempFrame.iFrameSizeY = 82;
	m_FrameMap.insert({ HURTGROUND, TempFrame });
}

void CArmEnemy::Attack()
{
	if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_FrontCollide, m_Target->Get_Collide()))
	{
		if (m_Target->Get_State() != ROLL && m_Target->Get_State() != HURTFLY && m_Target->Get_State() != HURTGROUND)
		{
			m_Target->Set_State(HURTFLY);
			if (m_Target->Get_Info()->fX < m_tInfo.fX)
				m_Target->Set_FrontAngle(0);
			else
				m_Target->Set_FrontAngle(PI);

			CSoundMgr::Get_Instance()->PlaySound(L"playerdie.wav", SOUND_EFFECT, 0.5f);
			CSoundMgr::Get_Instance()->PlaySound(L"punch_hit.wav", SOUND_EFFECT2, 0.5f);
		}
	}
}

void CArmEnemy::StateUpdate()
{
	INFO* fTargetInfo = m_Target->Get_Info();
	float fTargetX = fTargetInfo->fX;

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
		{
			if (m_bFollow)
			{
				if (m_tInfo.fX + 30.f < fTargetX)
				{
					m_State = RUN;
					m_fSpeed = 2.f;
				}
				else if (m_tInfo.fX - 30.f > fTargetX)
				{
					m_State = RUN;
					m_fSpeed = 2.f;
				}
			}
			else if (CheckTargetFront())
			{
				m_bFollow = true;
				m_State = RUN;
				m_fSpeed = 2.f;
			}
			else
			{
				if (m_WalkTime + 3000 < GetTickCount64())
				{
					m_State = WALK;
					m_fSpeed = 1.f;
					m_WalkTime = GetTickCount64();
				}
			}
		}
			
		break;

	case WALK:

		if (CheckTargetFront())
		{
			m_bFollow = true;
			m_State = RUN;
		}
		else
		{
			if (m_bJump || m_DirCheck[RIGHT] || m_DirCheck[LEFT])
			{
				m_tInfo.fX -= cos(m_fFrontAngle) * (m_fSpeed * 10.f);
				m_bFollow = false;
				if (m_fFrontAngle == 0)
					m_fFrontAngle = PI;
				else if (m_fFrontAngle == PI)
					m_fFrontAngle = 0;
			}
			else if (m_WalkTime + 4000 < GetTickCount64())
			{
				m_State = IDLE;
				m_WalkTime = GetTickCount64();
			}
			else if (m_fFrontAngle == 0)//go right
			{
				if (!m_bJump)
				{
					m_tInfo.fX += m_fSpeed;
				}
				else
					m_fFrontAngle = PI;
			}
			else if (m_fFrontAngle == PI)//go left
			{
				if (!m_bJump)
				{
					m_tInfo.fX -= m_fSpeed;
				}
				else
					m_fFrontAngle = 0;
			}
		}
		

		break;

	case RUN:
		if (m_bFollow)
		{
			if (m_bJump)
			{
				m_tInfo.fX -= cos(m_fFrontAngle) * (m_fSpeed + 2.f);
				m_bFollow = false;
				if (m_fFrontAngle == 0)
					m_fFrontAngle = PI;
				else if (m_fFrontAngle == PI)
					m_fFrontAngle = 0;

			}
			else
			{
				if (m_tInfo.fX + 30.f < fTargetX)
				{
					m_fFrontAngle = 0;
					if (!m_DirCheck[LEFT])
						m_tInfo.fX += cos(m_fFrontAngle) * (m_fSpeed);
				}
				else if (m_tInfo.fX - 30.f > fTargetX)
				{
					m_fFrontAngle = PI;
					if (!m_DirCheck[RIGHT])
						m_tInfo.fX += cos(m_fFrontAngle) * (m_fSpeed);
				}
				else if (m_tInfo.fX - 30.f <= fTargetX && m_tInfo.fX + 30.f >= fTargetX)
				{
					if (m_Target->Get_State() != HURTFLY && m_Target->Get_State() != HURTGROUND)
						m_State = ATTACK;
					else
						m_State = IDLE;
				}
			}
				
			
		}

		if (!CheckTargetFront())
		{
			m_State = IDLE;
			m_bFollow = false;
		}
			
		break;

	case ATTACK:
		if (m_Target->Get_State() != HURTFLY && m_Target->Get_State() != HURTGROUND)
		{
			if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd * 0.5)
				Attack();
		}
		else
			m_State = IDLE;	

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
			m_State = IDLE;
		break;

	case HURT:

		if (!m_HurtOn)
		{
			m_bJump = true;
			m_fSpeed_Vertical = 5.f;
			m_HurtOn = true;
		}
	
		if (m_bJump)
		{
			if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[LEFT])
				{
					if (!m_BulletHurt)
						m_tInfo.fX += 7.f * cos(m_fAttackAngle);
					else
						m_tInfo.fX += 4.f * cos(m_fFrontAngle + PI / 6.f);
				}

			}
			else if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[RIGHT])
				{
					if (!m_BulletHurt)
						m_tInfo.fX += 7.f * cos(m_fAttackAngle);
					else
						m_tInfo.fX += 4.f * cos(m_fFrontAngle + PI / 6.f);
				}
			}
		}
		

		if (m_FrameMap[m_State].iFrameStart >= m_FrameMap[m_State].iFrameEnd)
		{
			m_State = HURTGROUND;
			m_HurtOn = false;
		}
			//if (!m_bJump)
				//m_State = DEAD;
		break;

	case HURTGROUND:

		if (m_bJump)
		{
			if (m_fFrontAngle == PI)
			{
				if (!m_DirCheck[LEFT])
					m_tInfo.fX += 5.f * cos(m_fAttackAngle);
			}
			else if (m_fFrontAngle == 0)
			{
				if (!m_DirCheck[RIGHT])
					m_tInfo.fX += 5.f * cos(m_fAttackAngle);
			}
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))\
		{
			m_State = IDLE;
			m_bFollow = false;
			m_HurtOn = false;
			m_BulletHurt = false;
			m_WalkTime = GetTickCount64();
		}
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
		m_CheckCollide.right = m_FrontCollide.right +  m_CheckCWidth * 0.8;
	}
	else
	{
		m_CheckCollide.right = m_FrontCollide.right + m_CheckCWidth;
		m_CheckCollide.left = m_FrontCollide.left -  m_CheckCWidth * 0.8;
	}
	m_CheckCollide.top = m_FrontCollide.top - 60.f;
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
	OBJ_TYPE targetType = _target->Get_Type();
	
	if (targetType == ITEM)
	{
		if (m_State != HURT && m_State != HURTGROUND)
		{
			if (dynamic_cast<CItem*> (_target)->GetThrow())
				m_State = HURT;
		}
	}
	else if (targetType == GRABWALL || targetType == WALL)
	{
		m_DirCheck[_dir] = true;
		if (_dir == TOP)
			m_fSpeed_Vertical *= (-0.2f);
	}
	else if (targetType == BULLET)
	{
		if (m_State != HURT && m_State != HURTGROUND)
		{
			if (_target->GetOwner() != this && _target->GetOwner()->Get_Type() != ENEMY)
			{
				m_State = HURT;
				if (_dir == LEFT)
					m_fFrontAngle = 0;
				else if (_dir == RIGHT)
					m_fFrontAngle = PI;
				m_BulletHurt = true;
				_target->Set_State(DEAD);
				CSoundMgr::Get_Instance()->PlaySound(L"death_bullet.wav", SOUND_EFFECT, SOUND_VOL3);
			}
		}
	}
	
	return OBJ_NOEVENT;
}

int CArmEnemy::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
	}

	return OBJ_NOEVENT;
}

int CArmEnemy::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		m_DirCheck[_dir] = true;
	}
	return OBJ_NOEVENT;
}


