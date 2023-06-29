#include "stdafx.h"
#include "Honeycomb.h"
#include "LineMgr.h"
#include "Bee.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "BmpMgr.h"

CHoneycomb::CHoneycomb():m_dwTime(GetTickCount())
{
	m_Type = MONSTER;
	m_Collider_type = RECTANGLE;
}

CHoneycomb::~CHoneycomb()
{
	Release();
}

void CHoneycomb::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터6/Idle.gif", L"CombMonster_IDLE");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터6/Atk_1.gif", L"BasicMonster_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/몬스터6/Die.gif", L"CombMonster_DEAD");

	m_AnimMap.insert({ L"CombMonster_IDLE" , 6 });
	//m_AnimMap.insert({ L"BasicMonster_ATTACK" , 27 });
	m_AnimMap.insert({ L"CombMonster_DEAD" , 15 });

	AnimChange(L"CombMonster_IDLE");
	RatioFixByImage(m_AnimKey);
}

void CHoneycomb::Update(void)
{

	Gravity();

	StateUpdate();

	Create_Bee();

	CObj::Update_Rect();
}

void CHoneycomb::LateUpdate(void)
{
	for (auto iter = m_BeeList.begin(); iter != m_BeeList.end();) {
		if ((*iter)->Get_State()==DEAD) {
			iter = m_BeeList.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void CHoneycomb::Render(HDC hDC)
{
	CObj::Render(hDC);
}

void CHoneycomb::Release(void)
{
	m_BeeList.clear();
}

int CHoneycomb::OnCollision(CObj* _target, DIR _dir)
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

void CHoneycomb::Create_Bee()
{
	if (m_BeeList.size() < 2) {
		if (m_dwTime + 300 < GetTickCount()) {
			/*m_BeeList.push_back(CObjFactory<CBee>::Create(m_tInfo.fX + (m_BeeList.size() + 1) * 30,
				m_tInfo.fY + (m_BeeList.size() + 1) * 5), 320 * SMALL, 240 * SMALL);*/
			/*m_BeeList.push_back(CObjFactory<CBee>::Create((float)(m_tInfo.fX + (m_BeeList.size() + 1) * 30)
				,(float)(m_tInfo.fY + ((m_BeeList.size() + 1) * 5)), 320 * SMALL, 240 * SMALL));*/
			m_BeeList.push_back(CObjFactory<CBee>::Create(m_tInfo.fX+ (m_BeeList.size() + 1) * 30, m_tInfo.fY, 80 * SMALL, 60 * SMALL));
			CObjMgr::Get_Instance()->Add_Object(MONSTER, m_BeeList.back());
			m_dwTime = GetTickCount();
		}
	}
}

void CHoneycomb::Gravity()
{
	float fY = INFINITY;

	bool bLineCol = CLineMgr::Get_Instance()->CollisionLine(m_tInfo, &fY);

	if (bLineCol)
	{
		m_tInfo.fY = fY - 50;
	}
	else
	{
		m_tInfo.fY -= m_fSpeed_Vertical;
		m_fSpeed_Vertical -= (0.034f * G);
	}
}

void CHoneycomb::StateUpdate()
{
	switch (m_State)
	{
	case IDLE:
		m_AnimKey = L"CombMonster_IDLE";
		break;
	case BEFORE_DEAD:
		m_AnimKey = L"CombMonster_DEAD";
		if (m_AnimFrameIndex == m_AnimMap[m_AnimKey])
			m_State = DEAD;
		break;
	}
	m_AnimFrame = m_AnimMap[m_AnimKey];
}
