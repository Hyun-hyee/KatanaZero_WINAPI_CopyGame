#include "stdafx.h"
#include "BanBanAttack.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "BasicAttack.h"

CBanBanAttack::CBanBanAttack()
{
	m_Type = SKILL;
	m_Collider_type = RECTANGLE;
	CheckAttack = false;
	m_iLoop = 0;
}

CBanBanAttack::~CBanBanAttack()
{
}

void CBanBanAttack::Initialize()
{
}

void CBanBanAttack::Update()
{
	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();

	if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
	{
		if (m_iLoop > 5)
		{
			CheckAttack = false;
			m_State = DEAD;
		}
		++m_iLoop;
	}
}

void CBanBanAttack::LateUpdate(void)
{
}

void CBanBanAttack::Render(HDC hdc)
{
	//모든 캐릭터,스킬에 필요
	CObj::Render(hdc);

}

void CBanBanAttack::Release(void)
{
}

int CBanBanAttack::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (_target != m_Owner && TargetType != SKILL)
	{
		if (TargetType == PLAYER)
		{
			if (!CheckAttack)
			{
				Set_State(ATTACK);
				CheckAttack = true;
			}

			if (m_AnimFrameIndex == 0)
				Attack(_target);

			else if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] / 2 - 1)
				Attack(_target);

			else if (m_AnimFrameIndex == m_AnimMap[m_AnimKey] - 1)
				Attack(_target);
		}

	}

	return OBJ_NOEVENT;
}


void CBanBanAttack::SetOwnerImage()
{
	if (m_Owner != nullptr)
	{
		if (m_Owner->Get_Type() == MONSTER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/대현/반반Bullet.gif", L"SharkAttack_Monster");
			m_AnimMap.insert({ L"SharkAttack_Monster" , 12 });
			m_AnimKey = L"SharkAttack_Monster";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
		else if (m_Owner->Get_Type() == PLAYER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/대현/반반BulletEffect.gif", L"SharkAttack_Effect_Monster");
			m_AnimMap.insert({ L"SharkAttack_Effect_Monster" , 12 });
			m_AnimKey = L"SharkAttack_Effect_Monster";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
	}
}

void CBanBanAttack::Attack(CObj* _target)
{
	INFO* pTargetInfo = _target->Get_Info();
	CObj* pSkill = CObjFactory<CBasicAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 100.f, 100.f);
	pSkill->SetOwner(m_Owner);
	dynamic_cast<CBasicAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}
