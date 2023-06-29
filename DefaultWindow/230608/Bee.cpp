#include "stdafx.h"
#include "Bee.h"
#include "BmpMgr.h"
#include "ObjMgr.h"

CBee::CBee()
{
	ZeroMemory(&pTarget, sizeof(pTarget));
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
}

CBee::~CBee()
{
	Release();
}

void CBee::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터6/Idle_B.gif", L"Bee_IDLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터6/Die_B.gif", L"Bee_DEAD");

	m_AnimMap.insert({ L"Bee_IDLE" , 6 });
	//m_AnimMap.insert({ L"BasicMonster_ATTACK" , 27 });
	m_AnimMap.insert({ L"Bee_DEAD" , 15 });

	AnimChange(L"Bee_IDLE");
	RatioFixByImage(m_AnimKey);

	m_fSpeed = 3.f;

	pTarget = CObjMgr::Get_Instance()->Get_Player();
}

void CBee::Update(void)
{

	StateUpdate();

	float fWidth = (float)(pTarget->Get_Info()->fX - m_tInfo.fX);
	float fHeigh = (float)(pTarget->Get_Info()->fY - m_tInfo.fY);

	float fDiagonal = sqrtf(fWidth * fWidth + fHeigh * fHeigh);

	float m_Radian = acosf(fWidth / fDiagonal);

	if (pTarget->Get_Info()->fY > m_tInfo.fY) {
		m_Radian = (PI * 2) - m_Radian;
	}

	m_tInfo.fX += cosf(m_Radian) * m_fSpeed;
	m_tInfo.fY -= sinf(m_Radian) * m_fSpeed;

	CObj::Update_Rect();
}

void CBee::LateUpdate(void)
{
}

void CBee::Render(HDC hDC)
{
	CObj::Render(hDC);
}

void CBee::Release(void)
{
}

int CBee::OnCollision(CObj* _target, DIR _dir)
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

void CBee::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"Bee_IDLE";
		break;
	case BEFORE_DEAD:
		m_AnimKey = L"Bee_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
			m_State = DEAD;
		break;
	}
	m_AnimFrame = m_AnimMap[m_AnimKey];
}
