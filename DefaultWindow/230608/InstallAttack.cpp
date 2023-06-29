#include "stdafx.h"
#include "InstallAttack.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "BasicAttack.h"

CInstallAttack::CInstallAttack()
{
	m_Type = SKILL;
	m_Collider_type = RECTANGLE;
	CheckAttack = false;
	m_iLoop = 0;
}

CInstallAttack::~CInstallAttack()
{
}

void CInstallAttack::Initialize()
{
}

void CInstallAttack::Update()
{
	//RECT,Collide,FrontCollide ������Ʈ
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

void CInstallAttack::LateUpdate(void)
{
}

void CInstallAttack::Render(HDC hdc)
{
	//��� ĳ����,��ų�� �ʿ�
	CObj::Render(hdc);
}

void CInstallAttack::Release(void)
{
}

int CInstallAttack::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (_target != m_Owner && TargetType != SKILL)
	{
		if (TargetType == PLAYER || TargetType == MONSTER)
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

void CInstallAttack::SetOwnerImage()
{
	if (m_Owner != nullptr)
	{
		if (m_Owner->Get_Type() == MONSTER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/����/Skill_FireCircle.gif", L"InstallAttack_Monster");
			m_AnimMap.insert({ L"InstallAttack_Monster" , 12 });
			m_AnimKey = L"InstallAttack_Monster";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
		else if (m_Owner->Get_Type() == PLAYER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/����/Skill_BlueCircle.gif", L"InstallAttack");
			m_AnimMap.insert({ L"InstallAttack" , 12 });
			m_AnimKey = L"InstallAttack";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
	}
}

void CInstallAttack::Attack(CObj* _target)
{
	INFO* pTargetInfo = _target->Get_Info();
	CObj* pSkill = CObjFactory<CBasicAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 150.f, 150.f);
	pSkill->SetOwner(m_Owner);
	dynamic_cast<CBasicAttack*>(pSkill)->Set_Target(_target);
	dynamic_cast<CBasicAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}

