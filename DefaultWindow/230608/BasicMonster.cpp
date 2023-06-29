#include "stdafx.h"
#include "BasicMonster.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "CollisionManager.h"
#include "BasicAttack.h"

CBasicMonster::CBasicMonster()
{
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
	CheckOnLine = false;
	CheckAttack = false;
	m_Life = 100;
}

CBasicMonster::~CBasicMonster()
{
}

void CBasicMonster::Initialize(void)
{
	m_fSpeed = 6.f;

	int randAngle = rand() % 2;
	m_fFrontAngle = randAngle * PI;

	m_FrontCWidth = 80;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/혜진/1/Idle.gif", L"BasicMonster_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/혜진/1/Atk_1.gif", L"BasicMonster_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/혜진/1/Die.gif", L"BasicMonster_DEAD");

	m_AnimMap.insert({ L"BasicMonster_MOVE" , 8 });
	m_AnimMap.insert({ L"BasicMonster_ATTACK" , 27 });
	m_AnimMap.insert({ L"BasicMonster_DEAD" , 16 });

	m_AnimKey = L"BasicMonster_MOVE";
	AnimChange(m_AnimKey);
	RatioFixByImage(m_AnimKey);

}

void CBasicMonster::Update(void)
{
	if (m_State != BEFORE_DEAD)
		BasicAttack();

	Gravity();

	//상태 업데이트 (애니메이션 전환)
	StateUpdate();

	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
}

void CBasicMonster::LateUpdate(void)
{
}

void CBasicMonster::Render(HDC hDC)
{
	//모든 캐릭터에 필요
	CCharacter::Render(hDC);
	//모든 캐릭터,스킬에 필요
	CObj::Render(hDC);
}

void CBasicMonster::Release(void)
{
}

void CBasicMonster::Attack()
{
}

int CBasicMonster::OnCollision(CObj* _target, DIR _dir)
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
				m_Life -= 10.f;
		}

	}

	return OBJ_NOEVENT;
}

//각 객체의 해당 애니메이션 키값 넣어주기
void CBasicMonster::StateUpdate()
{
	if (m_State != DEAD)
	{
		if (m_State != BEFORE_DEAD)
		{
			switch (m_State)
			{
			case MOVE:
				m_AnimKey = L"BasicMonster_MOVE";
				break;

			case ATTACK:
				m_AnimKey = L"BasicMonster_ATTACK";
				if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
				{
					CheckAttack = false;
					m_State = MOVE;
				}
				break;
			}
		}

		switch (m_State)
		{
		case BEFORE_DEAD:
			m_AnimKey = L"BasicMonster_DEAD";
			if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
				m_State = DEAD;
			break;
		}
		AnimChange(m_AnimKey);
		//RatioFixByImage(m_AnimKey);
	}
}



void CBasicMonster::Gravity()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (bLineCol)
	{
		if (m_State == MOVE)
		{
			m_tInfo.fX += cos(m_fFrontAngle) * m_fSpeed;
		}
		m_tInfo.fY = fY - 50;
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
			CheckOnLine = false;
		}
		else
		{
			m_tInfo.fY -= m_fSpeed_Vertical;
			m_fSpeed_Vertical -= (0.034f * G);
			CheckOnLine = false;
		}
	}
}



void CBasicMonster::BasicAttack()
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
					CObj* pSkill = CObjFactory<CBasicAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 200.f, 200.f);
					pSkill->SetOwner(this);
					dynamic_cast<CBasicAttack*>(pSkill)->Set_Target(pTarget);
					dynamic_cast<CBasicAttack*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
					CheckAttack = true;
				}
				Set_State(ATTACK);
			}
		}
	}

}



