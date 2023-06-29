#include "stdafx.h"
#include "LaserAttack.h"
#include "BmpMgr.h"

CLaserAttack::CLaserAttack()
{
	m_Type = SKILL;
	m_Collider_type = RECTANGLE;
}

CLaserAttack::~CLaserAttack()
{
}

void CLaserAttack::Initialize()
{
}

void CLaserAttack::Update()
{
	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();

	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
		m_State = DEAD;
}

void CLaserAttack::LateUpdate(void)
{
}

void CLaserAttack::Render(HDC hdc)
{
	//모든 캐릭터,스킬에 필요
	CObj::Render(hdc);
}

void CLaserAttack::Release(void)
{
}

int CLaserAttack::OnCollision(CObj* _target, DIR _dir)
{
	return OBJ_NOEVENT;
}

void CLaserAttack::SetOwnerImage()
{
	if (m_Owner != nullptr)
	{
		if (m_Owner->Get_Type() == PLAYER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/대현/메가스매셔.gif", L"Smasher_Attack");
			m_AnimMap.insert({ L"Smasher_Attack" , 6 });
			m_AnimKey = L"Smasher_Attack";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
		else
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/대현/Effects.gif", L"Smasher_Monster");
			m_AnimMap.insert({ L"Smasher_Monster" , 5 });
			m_AnimKey = L"Smasher_Monster";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
	}

}
