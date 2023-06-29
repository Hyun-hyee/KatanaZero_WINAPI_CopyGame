#include "stdafx.h"
#include "BanBan.h"

#include "BmpMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "CollisionManager.h"

#include "BanBanAttack.h"


CBanBan::CBanBan()
{
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
	CheckOnLine = false;
	CheckAttack = false;
}

CBanBan::~CBanBan()
{
}

void CBanBan::Initialize(void)
{
	m_fJump_Power = 20.f;
	m_fJump_Angle = PI / 3;
	m_iAdditionJump_MaxCount = 3;
	m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
	m_fSpeed = 3.f;
	m_iHP = 1000;

	m_fMoveToNext = rand() % 5 + 1;
	m_iAtk = rand() % 2;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/반반/Idle.gif",  L"BanBan_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/반반/Move.gif",  L"BanBan_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/반반/ATK_1.gif", L"BanBan_ATTACK_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/반반/ATK_2.gif", L"BanBan_ATTACK_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/반반/ATK_3.gif", L"BanBan_ATTACK_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/반반/Die.gif",	 L"BanBan_DEAD");

	m_AnimMap.insert({ L"BanBan_IDLE" , 8 });
	m_AnimMap.insert({ L"BanBan_MOVE" , 8 });
	m_AnimMap.insert({ L"BanBan_ATTACK_1" , 26 });
	m_AnimMap.insert({ L"BanBan_ATTACK_2" , 26 });
	m_AnimMap.insert({ L"BanBan_ATTACK_3" , 26 });
	m_AnimMap.insert({ L"BanBan_DEAD" , 26 });

	AnimChange(L"BanBan_IDLE");
	RatioFixByImage(m_AnimKey);

}

void CBanBan::Update(void)
{
	Gravity();

	BanBan_1();

	//상태 업데이트 (애니메이션 전환)
	StateUpdate();

	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
}

void CBanBan::LateUpdate(void)
{
}

void CBanBan::Render(HDC hDC)
{

	CCharacter::Render(hDC);
	//모든 캐릭터,스킬에 필요
	CObj::Render(hDC);
}

void CBanBan::Release(void)
{
}

int CBanBan::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL) //누구랑 스킬
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER) //누구꺼 플레이어꺼
		{
			m_iHP -= 1;
			if (m_iHP <= 0 && m_State != BEFORE_DEAD && m_State != DEAD)
				Set_State(BEFORE_DEAD);
		}
	}

	return OBJ_NOEVENT;
}

//각 객체의 해당 애니메이션 키값 넣어주기
void CBanBan::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"BanBan_IDLE";
		break;

	case ATTACK:
		m_AnimKey = L"BanBan_ATTACK_2";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
		{
			CheckAttack = false;
			m_State = IDLE;
		}
		break;

	case BEFORE_DEAD:
		m_AnimKey = L"BanBan_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
			m_State = DEAD;
		break;
	}
	AnimChange(m_AnimKey);
}

void CBanBan::Gravity()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (bLineCol)
	{
		m_tInfo.fX += cos(m_fFrontAngle) * m_fSpeed;
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
		}
		else
		{
			m_tInfo.fY -= m_fSpeed_Vertical;
			m_fSpeed_Vertical -= (0.034f * G);
		}
	}

}

void CBanBan::BanBan_1()
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
					for (int i = 0; i < 10; i++) {

						INFO* pTargetInfo = pTarget->Get_Info();
						CObj* pSkill = CObjFactory<CBanBanAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 200.f, 200.f);
						pSkill->SetOwner(this);
						dynamic_cast<CBanBanAttack*>(pSkill)->SetOwnerImage();
						CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
						CheckAttack = true;

					}
				}
				Set_State(ATTACK);
			}
		}
	}

}