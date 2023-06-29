#include "stdafx.h"
#include "BasicAttack.h"
#include "BmpMgr.h"

CBasicAttack::CBasicAttack() : m_pTarget(nullptr)
{
	m_Type = SKILL;
	m_Collider_type = RECTANGLE;
}

CBasicAttack::~CBasicAttack()
{
}

void CBasicAttack::Initialize()
{
}

void CBasicAttack::Update()
{
	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();

	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
		m_State = DEAD;
}

void CBasicAttack::LateUpdate(void)
{
}

void CBasicAttack::Render(HDC hdc)
{
	//모든 캐릭터,스킬에 필요
	CObj::Render(hdc);
}

void CBasicAttack::Release(void)
{
}

int CBasicAttack::OnCollision(CObj* _target, DIR _dir)
{
	return OBJ_NOEVENT;
}

void CBasicAttack::SetOwnerImage()
{
	if (m_Owner != nullptr)
	{
		if (m_Owner->Get_Type() == PLAYER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/혜진/Skill_Basic.gif", L"Basic_Attack");
			m_AnimMap.insert({ L"Basic_Attack" , 6 });
			m_AnimKey = L"Basic_Attack";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
		else if (m_Owner->Get_Type() == MONSTER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/혜진/Skill_Basic_Monster.gif", L"Basic_Attack_Monster");
			m_AnimMap.insert({ L"Basic_Attack_Monster" , 5 });
			m_AnimKey = L"Basic_Attack_Monster";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
	}

}

