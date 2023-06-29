#include "stdafx.h"
#include "FollowBullet.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "ObjFactory.h"
#include "BasicAttack.h"

CFollowBullet::CFollowBullet()
{
	m_Type = SKILL;
	m_Collider_type = ELLIPSE;
	m_fDelay = 0;
	m_bUdateAngel = false;
	m_fAccel = 2.f;
	m_fTime = 1.f;
	m_OwnerType = OBJ_NONE;
}

CFollowBullet::~CFollowBullet()
{
	Release();
}

void CFollowBullet::Initialize(void)
{
	m_fSpeed = 12.f;
	m_fDelay = GetTickCount64();
}

void CFollowBullet::Update(void)
{
	m_tInfo.fX += cos(m_fAngle) * (m_fSpeed * m_fTime + 1 / 2 * m_fAccel * m_fTime);
	m_tInfo.fY -= sin(m_fAngle) * (m_fSpeed * m_fTime + 1 / 2 * m_fAccel * m_fTime);

	if (m_fDelay + 1000 < GetTickCount64())
	{
		if (!m_bUdateAngel)
		{
			m_bUdateAngel = true;
		}
	}

	if (m_bUdateAngel)
	{
		SelectTarget();
		++m_fTime;
	}
	//RECT,Collide,FrontCollide 업데이트
	CObj::Update_Rect();
}

void CFollowBullet::LateUpdate(void)
{
}

void CFollowBullet::Render(HDC hdc)
{
	//모든 캐릭터,스킬에 필요
	CObj::Render(hdc);
}

void CFollowBullet::Release(void)
{
}

int CFollowBullet::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();
	if (TargetType != SKILL)
	{
		if (TargetType != m_Owner->Get_Type())
			Set_State(DEAD);
	}

	if (_target != m_Owner && TargetType != SKILL)
	{
		if (TargetType == PLAYER || TargetType == MONSTER)
		{
			Attack(_target);
		}

	}

	return OBJ_NOEVENT;
}

void CFollowBullet::SetAngle(INFO* _Info)
{
	m_fAngle = (float)atan2((m_tInfo.fY - _Info->fY), (_Info->fX - m_tInfo.fX));
}

void CFollowBullet::SelectTarget()
{
	if (m_Owner->Get_State() != DEAD)
	{
		if (m_OwnerType == PLAYER)
		{
			list <CObj*>* MonsterList = CObjMgr::Get_Instance()->Get_ObjList(MONSTER);

			if (MonsterList->size() == 0)
				return;

			map<float, CObj*> DistanceMap = {};

			for (auto iter = MonsterList->begin(); iter != MonsterList->end(); ++iter)
			{
				INFO* targetInfo = (*iter)->Get_Info();
				float fDistance = pow(targetInfo->fX - m_tInfo.fX, 2.f) + pow(targetInfo->fY - m_tInfo.fY, 2.f);
				DistanceMap.insert({ fDistance,(*iter) });
			}

			m_target = DistanceMap.begin()->second;

		}
		else if (m_OwnerType == MONSTER)
		{
			m_target = CObjMgr::Get_Instance()->Get_Player();
		}
		SetAngle(m_target->Get_Info());
	}
}


void CFollowBullet::SetOwnerImage()
{
	if (m_Owner != nullptr)
	{
		if (m_Owner->Get_Type() == PLAYER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/혜진/Skill_BlueBall.gif", L"Follow_Bullet");
			m_AnimMap.insert({ L"Follow_Bullet" , 23 });
			m_AnimKey = L"Follow_Bullet";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
		else if (m_Owner->Get_Type() == MONSTER)
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/혜진/Skill_FireBall.gif", L"Follow_Bullet_Monster");
			m_AnimMap.insert({ L"Follow_Bullet_Monster" , 8 });
			m_AnimKey = L"Follow_Bullet_Monster";
			m_AnimFrame = m_AnimMap[m_AnimKey];
			AnimChange(m_AnimKey);
			RatioFixByImage(m_AnimKey);
		}
	}
	m_OwnerType = m_Owner->Get_Type();
}

void CFollowBullet::Attack(CObj* _target)
{
	INFO* pTargetInfo = _target->Get_Info();
	CObj* pSkill = CObjFactory<CBasicAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 150.f, 150.f);
	pSkill->SetOwner(m_Owner);
	dynamic_cast<CBasicAttack*>(pSkill)->Set_Target(_target);
	dynamic_cast<CBasicAttack*>(pSkill)->SetOwnerImage();
	CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
}
