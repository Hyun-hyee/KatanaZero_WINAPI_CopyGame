#include "stdafx.h"
#include "WatcherLight.h"

#include "BmpMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "CollisionManager.h"
#include "SharkAttack.h"

#include "Bullet.h"

CWatcherLight::CWatcherLight()
{
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
	CheckOnLine = false;
	CheckAttack = false;
}

CWatcherLight::~CWatcherLight()
{
}

void CWatcherLight::Initialize(void)
{
	m_fJump_Power = 20.f;
	m_fJump_Angle = PI / 3;
	m_iAdditionJump_MaxCount = 3;
	m_iAdditionJump_Count = m_iAdditionJump_MaxCount;
	m_fSpeed = 3.f;
	m_iHP = 300;

	m_fMoveToNext = rand() % 7 + 1;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����7/Idle.gif", L"WatcherLight_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����7/Move.gif", L"WatcherLight_MOVE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����7/Atk_1.gif", L"WatcherLight_ATTACK_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/����/����7/Die.gif", L"WatcherLight_DEAD");

	m_AnimMap.insert({ L"WatcherLight_IDLE" , 8 });
	m_AnimMap.insert({ L"WatcherLight_MOVE" , 8 });
	m_AnimMap.insert({ L"WatcherLight_ATTACK_1" , 16 });
	m_AnimMap.insert({ L"WatcherLight_DEAD" , 26 });

	AnimChange(L"WatcherLight_IDLE");
	RatioFixByImage(m_AnimKey);
}

void CWatcherLight::Update(void)
{
	SharkAttack();
	Gravity();
	//m_tInfo.fX += m_fSpeed;
	//if (m_tInfo.fX < 1850.f && m_tInfo.fX > 50.f)
	//{

	//	//1~4�� ���� �������� ���� �ߵ�
	//	if (dwTime + 3000 <= GetTickCount64())
	//	{
	//		m_bAtk = rand() % 1851 + 1;

	//		if (m_State == MOVE && m_State != DEAD)
	//		{
	//			if (m_bAtk <= m_tInfo.fX)
	//			{
	//				m_State == ATTACK;
	//			}
	//		}
	//	}
	//}
	//if (m_tInfo.fX >= 1850.f)
	//{
	//	m_State = MOVE;
	//	m_fSpeed *= -1.f;
	//	Set_FrontAngle(PI);
	//}
	//if (m_tInfo.fX <= 50.f)
	//{
	//	m_State = MOVE;
	//	m_fSpeed *= -1.f;
	//	Set_FrontAngle(0);
	//}



	//���� ������Ʈ (�ִϸ��̼� ��ȯ)
	StateUpdate();


	//RECT,Collide,FrontCollide ������Ʈ
	CObj::Update_Rect();
}

void CWatcherLight::LateUpdate(void)
{
}

void CWatcherLight::Render(HDC hDC)
{
	//��� ĳ���Ϳ� �ʿ�
	CCharacter::Render(hDC);
	//��� ĳ����,��ų�� �ʿ�
	CObj::Render(hDC);
}

void CWatcherLight::Release(void)
{
}

void CWatcherLight::Attack()
{

}

int CWatcherLight::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL) //������ ��ų
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER) //������ �÷��̾
		{
			m_iHP -= 1;
			if (m_iHP <= 0 && m_State != BEFORE_DEAD && m_State != DEAD)
				Set_State(BEFORE_DEAD);
		}
	}

	return OBJ_NOEVENT;
}

//�� ��ü�� �ش� �ִϸ��̼� Ű�� �־��ֱ�
void CWatcherLight::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"WatcherLight_IDLE";
		break;

	case ATTACK:
		m_AnimKey = L"WatcherLight_ATTACK_1";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
		{
			CheckAttack = false;
			m_State = IDLE;
		}		
		break;

	case BEFORE_DEAD:
		m_AnimKey = L"WatcherLight_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
			m_State = DEAD;
		break;
	}
	AnimChange(m_AnimKey);
}

void CWatcherLight::Gravity()
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


void CWatcherLight::SharkAttack()
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
					CObj* pSkill = CObjFactory<CSharkAttack>::Create(pTargetInfo->fX, pTargetInfo->fY, 200.f, 200.f);
					pSkill->SetOwner(this);
					dynamic_cast<CSharkAttack*>(pSkill)->SetOwnerImage();
					CObjMgr::Get_Instance()->Add_Object(SKILL, pSkill);
					CheckAttack = true;
				}
				Set_State(ATTACK);
			}
		}
	}

}

