#include "stdafx.h"
#include "GunEnemy.h"
#include "ObjFactory.h"
#include "BmpMgr.h"
#include "LineMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "Item.h"
#include "KeyMgr.h"
#include "Bullet.h"

CGunEnemy::CGunEnemy()
{
	m_Type = ENEMY;
	m_Collider_type = RECTANGLE;
}

CGunEnemy::~CGunEnemy()
{
	Release();
}

void CGunEnemy::Initialize(void)
{
	InitImage();
	m_State = WALK;
	m_fSpeed = 1.f;
	m_fAccel = 0.1f;
	m_CheckCWidth = 150.f;
	m_bFollow = false;
	m_HurtOn = false;
	m_WalkTime = GetTickCount64();
	m_fFrontCWidth = 450.f;
	m_AttackOn = false;
	m_BulletOn = false;
	m_BulletHurt = false;

	m_Target = CObjMgr::Get_Instance()->Get_Player();
}

void CGunEnemy::Update(void)
{
	StateUpdate();

	if (g_SlowMotion)
	{
		m_fSpeed = 0.5f;
		m_SlowTime = 5000;
	}
	else
		m_SlowTime = 0;

	Jump();
	//RECT,Collide,FrontCollide 업데이트
	__super::Update_Rect();
	Update_CheckCollide();

	CObj::Move_Frame();
}

void CGunEnemy::LateUpdate(void)
{
}

void CGunEnemy::Render(HDC hDC)
{
	CObj::FrameRender(hDC);

	CObj::CollideRender(hDC);
	CObj::CollideRender(hDC, m_CheckCollide);
}

void CGunEnemy::Release(void)
{
}

void CGunEnemy::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_cop_idle_8x2.bmp",   L"GunEnemy_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_cop_walk_8x2.bmp",  L"GunEnemy_WALK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_cop_run_10x2.bmp",   L"GunEnemy_RUN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_cop_idle_8x2.bmp", L"GunEnemy_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_cop_hurt_14x2.bmp",  L"GunEnemy_HURT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_cop_hurtground.png", L"GunEnemy_HURTGROUND");

	FRAME TempFrame;
	TempFrame.AnimKey = L"GunEnemy_IDLE";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 72;
	TempFrame.iFrameSizeY = 74;
	m_FrameMap.insert({ IDLE, TempFrame });

	TempFrame.AnimKey = L"GunEnemy_WALK";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd =7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 54;
	TempFrame.iFrameSizeY = 76;
	m_FrameMap.insert({ WALK, TempFrame });

	TempFrame.AnimKey = L"GunEnemy_RUN";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 9;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 90;
	TempFrame.iFrameSizeY = 80;
	m_FrameMap.insert({ RUN, TempFrame });

	TempFrame.AnimKey = L"GunEnemy_ATTACK";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 7;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 60;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 72;
	TempFrame.iFrameSizeY = 74;
	m_FrameMap.insert({ ATTACK, TempFrame });

	TempFrame.AnimKey = L"GunEnemy_HURT";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 13;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 90;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 82;
	TempFrame.iFrameSizeY = 68;
	m_FrameMap.insert({ HURT, TempFrame });

	TempFrame.AnimKey = L"GunEnemy_HURTGROUND";
	TempFrame.iFrameStart = 0;
	TempFrame.iFrameEnd = 0;
	TempFrame.iMotion = 0;
	TempFrame.dwSpeed = 90;
	TempFrame.dwTime = GetTickCount64();
	TempFrame.iFrameSizeX = 82;
	TempFrame.iFrameSizeY = 68;
	m_FrameMap.insert({ HURTGROUND, TempFrame });
}

void CGunEnemy::Attack()
{
	CObj* Temp = CObjFactory<CBullet>::Create();
	Temp->Set_Pos(m_tInfo.fX + cos(m_fFrontAngle) * 10.f, m_tInfo.fY - 5.f);
	Temp->Set_FrontAngle(m_fFrontAngle);
	Temp->Set_AttackAngle(m_fFrontAngle);
	Temp->SetOwner(this);
	CObjMgr::Get_Instance()->Add_Object(BULLET, Temp);

}

void CGunEnemy::StateUpdate()
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
		if (m_AttackOn)
		{
			if (m_AttackTime + 5000 + m_SlowTime < GetTickCount64())
			{
				m_AttackOn = false;
			}
		}
		else
		{
			if (m_bFollow)
			{
				if (m_tInfo.fX + 30.f < fTargetX)
				{
					m_fSpeed = 2.f;
					m_State = RUN;
				}
				else if (m_tInfo.fX - 30.f > fTargetX)
				{
					m_fSpeed = 2.f;
					m_State = RUN;
				}
			}
			else if (CheckTargetFront())
			{
				m_bFollow = true;
				m_fSpeed = 2.f;
				m_State = RUN;
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
			if (m_fFrontAngle == 0)//go right
			{
				if (!m_DirCheck[RIGHT] && !m_bJump)
				{
					m_tInfo.fX += m_fSpeed;
				}
				else
					m_fFrontAngle = PI;
			}
			else if (m_fFrontAngle == PI)//go left
			{
				if (!m_DirCheck[LEFT] && !m_bJump)
				{
					m_tInfo.fX -= m_fSpeed;
				}
				else
					m_fFrontAngle = 0;
			}

			if (m_bJump)
				m_tInfo.fX += cos(m_fFrontAngle) * m_fSpeed;

			if (m_WalkTime + 4000 < GetTickCount64())
			{
				m_State = IDLE;
				m_WalkTime = GetTickCount64();
			}
		}


		break;

	case RUN:
		if (m_bFollow)
		{
			if (m_bJump)
			{
				m_tInfo.fX -= cos(m_fFrontAngle) * (m_fSpeed + 2.f);
				m_State = IDLE;
				m_bFollow = false;
				if (m_fFrontAngle == 0)
					m_fFrontAngle = PI;
				else if (m_fFrontAngle == PI)
					m_fFrontAngle = 0;
			}
			else
			{
				if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_FrontCollide, m_Target->Get_Collide()))
				{
					if (m_Target->Get_State() != HURTFLY && m_Target->Get_State() != HURTGROUND)
					{
						m_State = ATTACK;
						m_AttackOn = true;
						m_BulletOn = true;
					}
					else
						m_State = IDLE;
				}
				else if (m_tInfo.fX + 30.f < fTargetX)
				{
					m_fFrontAngle = 0;
					m_tInfo.fX += cos(m_fFrontAngle) * (m_fSpeed);
				}
				else if (m_tInfo.fX - 30.f > fTargetX)
				{
					m_fFrontAngle = PI;
					m_tInfo.fX += cos(m_fFrontAngle) * (m_fSpeed);
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
		if (m_AttackOn && m_Target->Get_State() != HURTFLY && m_Target->Get_State() != HURTGROUND)
		{
			if (m_BulletOn)
			{
				if(m_FrameMap[m_State].iFrameStart == 3)
				{
					Attack();
					m_AttackTime = GetTickCount64();
					m_BulletOn = false;
				}
			}
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

		if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
		{
			m_State = IDLE; 
			m_bFollow = false;
			m_HurtOn = false;
			m_WalkTime = GetTickCount64();
			m_AttackOn = false;
			m_BulletOn = false;
			m_BulletHurt = false;
		}
	}

}

void CGunEnemy::Jump()
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

void CGunEnemy::Update_CheckCollide()
{
	if (m_fFrontAngle == PI)
	{
		m_CheckCollide.left = m_FrontCollide.left - m_CheckCWidth;
		m_CheckCollide.right = m_FrontCollide.right + m_CheckCWidth * 0.8;
	}
	else
	{
		m_CheckCollide.right = m_FrontCollide.right + m_CheckCWidth;
		m_CheckCollide.left = m_FrontCollide.left - m_CheckCWidth * 0.8;
	}
	m_CheckCollide.top = m_FrontCollide.top - 60.f;
	m_CheckCollide.bottom = m_FrontCollide.bottom;
}

bool CGunEnemy::CheckTargetFront()
{
	if (DIR_NONE != CCollisionMgr::Get_Instance()->Collision_Enter_SS(&m_CheckCollide, m_Target->Get_Collide()))
		return true;
	else
		return false;
}

int CGunEnemy::InCollision(CObj* _target, DIR _dir)
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
			if (_target->GetOwner() != this)
			{
				m_State = HURT;
				if (_dir == LEFT)
					m_fFrontAngle = 0;
				else if (_dir == RIGHT)
					m_fFrontAngle = PI;
				m_BulletHurt = true;
				CSoundMgr::Get_Instance()->PlaySound(L"death_bullet.wav", SOUND_EFFECT, SOUND_VOL3);

			}
		}				
	}

	return OBJ_NOEVENT;
}

int CGunEnemy::OutCollision(CObj* _target)
{
	OBJ_TYPE targetType = _target->Get_Type();

	if (targetType == GRABWALL || targetType == WALL)
	{
		for (int i = 0; i < DIR_END; ++i)
			m_DirCheck[i] = false;
	}

	return OBJ_NOEVENT;
}

int CGunEnemy::OnCollision(CObj* _target)
{
	return OBJ_NOEVENT;
}


