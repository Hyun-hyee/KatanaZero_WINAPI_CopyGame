#include "stdafx.h"
#include "Imp.h"

#include "BmpMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "CollisionManager.h"
#include "BasicAttack.h"

CImp::CImp() : iImpHp(5)
{
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
}

CImp::~CImp()
{
}

void CImp::Initialize(void)
{
	m_fSpeed = 6.f;
	int					iImpHp;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/몬스터1/Idle.gif",  L"Imp_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/몬스터1/Atk_1.gif", L"Imp_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/대현/몬스터1/Die.gif",   L"Imp_DEAD");

	m_AnimMap.insert({ L"Imp_IDLE" , 8 });
	m_AnimMap.insert({ L"Imp_ATTACK" , 27 });
	m_AnimMap.insert({ L"Imp_DEAD" , 9 });

	AnimChange(L"Imp_IDLE");
	RatioFixByImage(m_AnimKey);

}

void CImp::Update(void)
{
	Gravity();


	if (m_tInfo.fY <= 970.f)
	{
		m_tInfo.fY -= 50;
	}

	//상태 업데이트 (애니메이션 전환)
	StateUpdate();

	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
}

void CImp::LateUpdate(void)
{
}

void CImp::Render(HDC hDC)
{
	//모든 캐릭터에 필요
	CCharacter::Render(hDC);
	//모든 캐릭터,스킬에 필요
	CObj::Render(hDC);
}

void CImp::Release(void)
{
}

int CImp::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL) //누구랑 스킬
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER) //누구꺼 플레이어꺼
			Set_State(DEAD);
	}
	if (TargetType == PLAYER) //누구랑 스킬
	{
		iImpHp--;
		if (iImpHp <= 0.f) //누구꺼 플레이어꺼
			Set_State(BEFORE_DEAD);
	}
	return OBJ_NOEVENT;
}


void CImp::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"Imp_IDLE";
		break;

	case ATTACK:
		m_AnimKey = L"Imp_ATTACK";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
		{
			m_State = IDLE;
		}		
		break;

	case BEFORE_DEAD:
		m_AnimKey = L"Imp_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
			m_State = DEAD;
		break;
	}
	AnimChange(m_AnimKey);
}

void CImp::Gravity()
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
