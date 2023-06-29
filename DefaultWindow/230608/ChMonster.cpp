#include "stdafx.h"
#include "ChMonster.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "LineMgr.h"
#include "CollisionManager.h"



CChMonster::CChMonster() :m_Move_Range_LX(0.f), m_Move_Range_RX(0.f)
{

	/*ZeroMemory(&m_ChangeLeft, sizeof(m_ChangeLeft));
	ZeroMemory(&m_ChangeRight, sizeof(m_ChangeRight));*/
	ZeroMemory(&pTarget, sizeof(pTarget));

	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;

}

CChMonster::~CChMonster()
{
	Release();
}

void CChMonster::Initialize()
{
	m_fSpeed = 3.f;
	m_fAccel = 15.f;

	pTarget = CObjMgr::Get_Instance()->Get_Player();

	

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터8/IDLE.gif", L"ChMonster_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터8/ATTACK.gif", L"ChMonster_ATK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터8/DEAD.gif", L"ChMonster_DEAD");

	m_AnimMap.insert({ L"ChMonster_IDLE" , 6 });
	m_AnimMap.insert({ L"ChMonster_ATK" , 6 });
	m_AnimMap.insert({ L"ChMonster_DEAD" , 13 });

	AnimChange(L"ChMonster_IDLE");
	RatioFixByImage(m_AnimKey);
	Set_FrontWidth(200.f);

}

void CChMonster::Update()
{

	
	Gravity();

	StateUpdate();

	if (m_tRect.right >= m_Move_Range_RX) {
		m_fSpeed = -1.f * m_fSpeed;
		m_fAccel = -1.f * m_fAccel;
		Set_FrontAngle(PI);
	}
	else if (m_tRect.left <= m_Move_Range_LX) {
		m_fSpeed = -1.f * m_fSpeed;
		m_fAccel = -1.f * m_fAccel;

		Set_FrontAngle(0);
	}

	if (CCollisionManager::Collision_Enter_SS(&m_FrontCollide, pTarget->Get_Collide()) != DIR_NONE) {
		m_tInfo.fX += m_fAccel;
		m_State = ATTACK;
	}
	else {
		m_tInfo.fX += m_fSpeed;
		m_State = IDLE;
	}



	CObj::Update_Rect();
}

void CChMonster::LateUpdate(void)
{
	//Set_FrontCollide(m_tRect.left * m_ChangeLeft, m_tRect.top, m_tRect.right * m_ChangeRight, m_tRect.bottom);

	

}

void CChMonster::Render(HDC hdc)
{

	//모든 캐릭터,스킬에 필요
	//CCharacter::Render(hdc);
	CObj::Render(hdc);


}

void CChMonster::Release(void)
{
}

int CChMonster::OnCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE TargetType = _target->Get_Type();

	if (TargetType == SKILL)
	{
		OBJ_TYPE TargetOwnerType = _target->GetOwner()->Get_Type();
		if (TargetOwnerType == PLAYER)
			Set_State(BEFORE_DEAD);
	}

	return OBJ_NOEVENT;
}

void CChMonster::Gravity()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);


	if (bLineCol)
	{
		m_tInfo.fY = fY - 60;
	}
	else
	{
		m_tInfo.fY -= m_fSpeed_Vertical;
		m_fSpeed_Vertical -= (0.034f * G);
	}
}

void CChMonster::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"ChMonster_IDLE";
		break;

	case ATTACK:
		m_AnimKey = L"ChMonster_ATK";
		break;
	case BEFORE_DEAD:
		m_AnimKey = L"ChMonster_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey]);
		m_State = DEAD;
		break;
	}
	m_AnimFrame = m_AnimMap[m_AnimKey];
}
