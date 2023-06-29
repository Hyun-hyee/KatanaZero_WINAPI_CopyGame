#include "stdafx.h"
#include "FollowBoss.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "CollisionManager.h"
#include "InstallAttack.h"
#include "BasicAttack.h"
#include "FollowBullet.h"

CFollowBoss::CFollowBoss() : m_bStateChanged(false), dwStateTimer(GetTickCount64())
{
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
	CheckOnLine = false;
	CheckAttack = false;
	m_Life = 900;
}

CFollowBoss::~CFollowBoss()
{
}

void CFollowBoss::Initialize(void)
{
	m_fSpeed = 4.f;

	int randAngle = rand() % 2;
	m_fFrontAngle = randAngle * PI;

	m_FrontCWidth = 80;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/혜진/2/Move_ref.gif", L"FollowMonster_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/혜진/2/Atk_ref.gif", L"FollowMonster_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/혜진/2/Die_ref.gif", L"FollowMonster_DEAD");

	m_AnimMap.insert({ L"FollowMonster_MOVE" , 22 });
	m_AnimMap.insert({ L"FollowMonster_ATTACK" , 27 });
	m_AnimMap.insert({ L"FollowMonster_DEAD" , 20 });

	m_AnimKey = L"FollowMonster_MOVE";
	AnimChange(m_AnimKey);
	RatioFixByImage(m_AnimKey);


	m_mFnAction.emplace(STATE::IDLE, &CFollowBoss::Idle);
	m_mFnAction.emplace(STATE::MOVE_TO_ENEMY, &CFollowBoss::MoveToEnemy);
	m_mFnAction.emplace(STATE::MOVE_TO_ANYWHERE, &CFollowBoss::MoveToAnywhere);
	m_mFnAction.emplace(STATE::MOVE_TO_LEFT, &CFollowBoss::MoveToRight);
	m_mFnAction.emplace(STATE::MOVE_TO_RIGHT, &CFollowBoss::MoveToLeft);
	m_mFnAction.emplace(STATE::MOVE_TO_CENTER, &CFollowBoss::MoveToCenter);
	m_mFnAction.emplace(STATE::ATTACK_FOLLOWB1, &CFollowBoss::Attack_FollowB1);
	m_mFnAction.emplace(STATE::ATTACK_FOLLOWB2, &CFollowBoss::Attack_FollowB2);
	m_mFnAction.emplace(STATE::ATTACK_FOLLOWB3, &CFollowBoss::Attack_FollowB3);
	m_mFnAction.emplace(STATE::ATTACK_INSTALL1, &CFollowBoss::Attack_Install);
	m_mFnAction.emplace(STATE::ATTACK_INSTALL2, &CFollowBoss::Attack_Install2);
	m_mFnAction.emplace(STATE::DIE, &CFollowBoss::Die);
	m_mFnAction.emplace(STATE::SET_TARGET, &CFollowBoss::SetTarget);

	m_eState = STATE::SET_TARGET;
}

void CFollowBoss::Update(void)
{
	if (m_State != BEFORE_DEAD)
	{
		int randSkill = rand() % 2;
		if (randSkill == 1)
			InstallAttack();
		else
			FollowBullet();
	}
	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
	{
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fX >= m_tInfo.fX)
			m_fFrontAngle = 0;
		else
			m_fFrontAngle = PI;
	}
	Gravity();

	//상태 업데이트 (애니메이션 전환)
	if (m_Life < 0)
		m_State = BEFORE_DEAD;

	StateUpdate();

	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
}

void CFollowBoss::LateUpdate(void)
{
}

void CFollowBoss::Render(HDC hDC)
{
	//모든 캐릭터에 필요
	CCharacter::Render(hDC);
	//모든 캐릭터,스킬에 필요
	CObj::Render(hDC);
}

void CFollowBoss::Release(void)
{
}

void CFollowBoss::Attack()
{
}

int CFollowBoss::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL || TargetType == EFFECT)
	{
		if (_target->GetOwner()->Get_Type() != MONSTER)
		{
			if (TargetType == EFFECT)
			{
				if (this != dynamic_cast<CBasicAttack*>(_target)->Get_Target())
					return OBJ_NOEVENT;
			}

			if (m_Life <= 0)
			{
				OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
				if (TargetOwnerType == PLAYER)
					Set_State(BEFORE_DEAD);
			}
			else
				m_Life -= 1.f;
		}
	}
	return OBJ_NOEVENT;
}

//각 객체의 해당 애니메이션 키값 넣어주기
void CFollowBoss::StateUpdate()
{

	if (m_State != BEFORE_DEAD)
		m_mFnAction[m_eState](this);

	AnimChange(m_AnimKey);
	/*if (m_State != DEAD)
	{
		if (m_State != BEFORE_DEAD)
		{
			switch (m_State)
			{
			case MOVE:
				m_AnimKey = L"FollowMonster_MOVE";
				break;

			case ATTACK:
				m_AnimKey = L"FollowMonster_ATTACK";
				if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
				{
					CheckAttack = false;
					m_State = MOVE;
				}
				break;
			}
		}
		*/
		switch (m_State)
		{
		case BEFORE_DEAD:
			m_AnimKey = L"FollowMonster_DEAD";
			if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
				m_State = DEAD;
			break;
		}

}



void CFollowBoss::Gravity()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (bLineCol)
	{
		if (m_State == MOVE)
		{
			m_tInfo.fX += cos(m_fFrontAngle) * m_fSpeed;
			m_tInfo.fY = fY - 50;
		}
		CheckOnLine = true;
	}
	else
	{
		if (CheckOnLine)
		{
			if (m_fFrontAngle == 0)
				m_fFrontAngle = PI;
			else if (m_fFrontAngle == PI)
				m_fFrontAngle = 0;

			m_tInfo.fX += cos(m_fFrontAngle) * m_fSpeed;
		}
		else
		{
			if (m_State == MOVE)
			{
				m_tInfo.fY -= m_fSpeed_Vertical;
				m_fSpeed_Vertical -= (0.034f * G);
			}
		}
	}
}



void CFollowBoss::InstallAttack()
{
	CObj* pTarget = CObjMgr::Get_Instance()->Get_Player();
	RECT* pTargetCollide = pTarget->Get_Collide();

	if (pTarget != nullptr)
	{
		if (DIR_NONE != CCollisionManager::Collision_Enter_SS(&m_FrontCollide, pTargetCollide))
		{
			if (!CheckAttack)
			{
				if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2)
				{
					INFO* pTargetInfo = pTarget->Get_Info();
					CObj* pSkill = CObjFactory<CInstallAttack>::Create(pTargetInfo->fX, pTargetInfo->fY - 100, 500 * SMALL, 350 * SMALL);
					pSkill->SetOwner(this);
					dynamic_cast<CInstallAttack*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
					CheckAttack = true;
				}
				Set_State(ATTACK);
			}
		}
	}

}


void CFollowBoss::FollowBullet()
{
	CObj* pTarget = CObjMgr::Get_Instance()->Get_Player();
	RECT* pTargetCollide = pTarget->Get_Collide();

	if (pTarget != nullptr)
	{
		if (DIR_NONE != CCollisionManager::Collision_Enter_SS(&m_FrontCollide, pTargetCollide))
		{
			if (!CheckAttack)
			{
				if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2)
				{
					CObj* pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
					pSkill->SetOwner(this);
					dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI / 6);
					dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

					pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
					pSkill->SetOwner(this);
					dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 2 / 6);
					dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

					pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
					pSkill->SetOwner(this);
					dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 3 / 6);
					dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

					pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
					pSkill->SetOwner(this);
					dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 4 / 6);
					dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

					pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
					pSkill->SetOwner(this);
					dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 5 / 6);
					dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

					pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
					pSkill->SetOwner(this);
					dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 6 / 6);
					dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
					CheckAttack = true;
				}
				Set_State(ATTACK);
			}
		}
	}

}

void CFollowBoss::Idle()
{
	AnimChange(L"FollowMonster_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
	}

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		// 메테오 공격
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
		// 땅으로 들어가
		else
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
	}
}

void CFollowBoss::MoveToEnemy()
{
	AnimChange(L"FollowMonster_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = MOVE;
		if (CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fX >= m_tInfo.fX)
			m_fFrontAngle = 0;
		else
			m_fFrontAngle = PI;
	}
	

	
	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::ATTACK_FOLLOWB1);
		}
		else
		{
			StateChange(STATE::ATTACK_INSTALL1);
		}
	}
}

void CFollowBoss::MoveToAnywhere()
{
	AnimChange(L"FollowMonster_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = MOVE;
	}

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::ATTACK_FOLLOWB2);
		}
		else
		{
			StateChange(STATE::ATTACK_INSTALL1);
		}
	}
}

void CFollowBoss::MoveToRight()
{
	AnimChange(L"FollowMonster_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = IDLE;
		m_fFrontAngle = PI;
		m_tInfo.fX = 1650;
	}
	m_fFrontAngle = 0;
	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::ATTACK_FOLLOWB2);
		}
		else
		{
			StateChange(STATE::ATTACK_INSTALL1);
		}
	}
}

void CFollowBoss::MoveToLeft()
{
	AnimChange(L"FollowMonster_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = IDLE;
		m_fFrontAngle = 0;
		m_tInfo.fX = 200;
	}
	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::ATTACK_FOLLOWB2);
		}
		else
		{
			StateChange(STATE::ATTACK_INSTALL1);
		}
	}
}

void CFollowBoss::MoveToCenter()
{
	AnimChange(L"FollowMonster_MOVE");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = IDLE;
		m_tInfo.fX = 1000;
	}

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::ATTACK_FOLLOWB2);
		}
		else
		{
			StateChange(STATE::ATTACK_INSTALL1);
		}
	}
}

void CFollowBoss::Attack_FollowB1()
{
	AnimChange(L"FollowMonster_ATTACK");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = ATTACK;
	}

	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2)
	{
		CObj* pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 2 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 3 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 4 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 5 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 6 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
		CheckAttack = true;
	}

	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2 + 2)
	{
		float A;
		if (m_fFrontAngle == PI)
			A = 0;
		else
			A = PI;

		CObj* pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(A) * PI / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(A) * PI * 2 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(A) * PI * 3 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(A) * PI * 4 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(A) * PI * 5 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(A) * PI * 6 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
		CheckAttack = true;
	}


	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CFollowBoss::Attack_FollowB2()
{
	AnimChange(L"FollowMonster_ATTACK");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = ATTACK;
	}
	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2)
	{
		CObj* pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 2 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 3 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 4 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 5 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle) * PI * 6 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
		CheckAttack = true;

		/****************/
		
		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle + PI) * PI / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle + PI) * PI * 2 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle + PI) * PI * 3 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle + PI) * PI * 4 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle + PI) * PI * 5 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

		pSkill = CObjFactory<CFollowBullet>::Create(m_tInfo.fX, m_tInfo.fY - 100, 100, 100);
		pSkill->SetOwner(this);
		dynamic_cast<CFollowBullet*>(pSkill)->SetAngle(m_fFrontAngle + cos(m_fFrontAngle + PI) * PI * 6 / 6);
		dynamic_cast<CFollowBullet*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
		CheckAttack = true;
	}

	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CFollowBoss::Attack_FollowB3()
{
}

void CFollowBoss::Attack_Install()
{
	AnimChange(L"FollowMonster_ATTACK");

	// 최초 패턴 들어옴.
	if (IsStateChanged())
	{
		m_iPatternRandom = rand() % 10;
		dwStateTimer = GetTickCount64();
		m_State = ATTACK;
	}
	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2)
	{
		INFO* pTargetInfo = m_pTarget->Get_Info();
		CObj* pSkill = CObjFactory<CInstallAttack>::Create(pTargetInfo->fX, pTargetInfo->fY - 100, 500 * SMALL, 350 * SMALL);
		pSkill->SetOwner(this);
		dynamic_cast<CInstallAttack*>(pSkill)->SetOwnerImage();
		CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);

	}
	// 시간 지나면 패턴 바뀜
	if (dwStateTimer + (ULONGLONG)1500 < GetTickCount64())
	{
		if (m_iPatternRandom >= 4)
		{
			StateChange(STATE::MOVE_TO_ANYWHERE);
		}
		else
		{
			StateChange(STATE::MOVE_TO_ENEMY);
		}
	}
}

void CFollowBoss::Attack_Install2()
{
}

void CFollowBoss::Die()
{
}

void CFollowBoss::SetTarget()
{
	if (nullptr == m_pTarget)
	{
		m_pTarget = CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front();
	}


	StateChange(STATE::IDLE);
}

