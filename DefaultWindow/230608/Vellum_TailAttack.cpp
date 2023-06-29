#include "stdafx.h"
#include "Vellum_TailAttack.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "BasicAttack.h"

CVellum_TailAttack::CVellum_TailAttack()
{
}

CVellum_TailAttack::~CVellum_TailAttack()
{
}

void CVellum_TailAttack::Initialize()
{
}

void CVellum_TailAttack::Update()
{
	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();

	if (m_AnimFrameIndex >= m_AnimMap[m_AnimKey]-1)
	{
		m_State = DEAD;
	}
}

void CVellum_TailAttack::LateUpdate(void)
{
}

void CVellum_TailAttack::Render(HDC hdc)
{
	CObj::Render(hdc);
}

void CVellum_TailAttack::Release(void)
{
}

int CVellum_TailAttack::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (_target != m_Owner && TargetType != SKILL)
	{
		if (TargetType == PLAYER)
		{
			if (!CheckAttack)
			{
				if (m_AnimFrameIndex == 20)
					Attack(_target);

				CheckAttack = true;
			}
		}
	}

	return OBJ_NOEVENT;
}


void CVellum_TailAttack::SetOwnerImage()
{
	if (m_Owner != nullptr)
	{
		if (m_Owner->Get_Type() == MONSTER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/학준/벨룸/skill/tail_attack.gif", L"Vellum_FX_TailAttack");
			m_AnimMap.insert({ L"Vellum_FX_TailAttack" , 41 });
			AnimChange(L"Vellum_FX_TailAttack");
			RatioFixByImage(m_AnimKey);
		}
		else if (m_Owner->Get_Type() == PLAYER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/학준/벨룸/skill/tail_attack.gif", L"Vellum_FX_TailAttack");
			m_AnimMap.insert({ L"Vellum_FX_TailAttack" , 41 });
			AnimChange(L"Vellum_FX_TailAttack");
			RatioFixByImage(m_AnimKey);
		}
	}
}

void CVellum_TailAttack::Attack(CObj* _target)
{
	INFO* pTargetInfo = _target->Get_Info();
	CObj* pSkill = CObjFactory<CBasicAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 100.f, 100.f);
	pSkill->SetOwner(m_Owner);
	dynamic_cast<CBasicAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}
