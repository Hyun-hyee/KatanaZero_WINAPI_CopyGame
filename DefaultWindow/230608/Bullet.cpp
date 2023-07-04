#include "stdafx.h"
#include "Bullet.h"
#include "BmpMgr.h"

CBullet::CBullet()
{
	m_Type = BULLET;
	m_Collider_type = RECTANGLE;
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 2.f;
	m_fSpeed = 20.f;
	InitImage();
	m_OneImgKey = L"Bullet_R";
}

void CBullet::Update()
{
	if (m_fFrontAngle == 0)
		m_OneImgKey = L"Bullet_R";
	else if (m_fFrontAngle == PI)
		m_OneImgKey = L"Bullet_L";

	if (g_SlowMotion)
	{
		m_fSpeed = 0.8f;
	}
	else
		m_fSpeed = 10.f;

	__super::Update_Rect();

	m_tInfo.fX += cos(m_fAttackAngle) * m_fSpeed;
	m_tInfo.fY -= sin(m_fAttackAngle) * m_fSpeed;
}

void CBullet::LateUpdate(void)
{
}

void CBullet::Render(HDC hdc)
{
	CollideRender(hdc);
	CObj::BasicRender(hdc);
}

void CBullet::Release(void)
{
}

void CBullet::InitImage()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_bullet_L.png", L"Bullet_L");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Resource/images/enemy/enemy_bullet_R.png", L"Bullet_R");
}

int CBullet::InCollision(CObj* _target, DIR _dir)
{
	OBJ_TYPE targetType = _target->Get_Type();
	
	if (m_Owner != _target)
	{
		if (targetType == WALL || targetType == GRABWALL || targetType == ENEMY || targetType == PLAYER)
		{
			Set_State(DEAD);
		}
	}
	return OBJ_NOEVENT;
}

int CBullet::OutCollision(CObj* _target)
{
	return 0;
}

int CBullet::OnCollision(CObj* _target)
{
	return 0;
}

//void CBullet:: SetAttackAngle()
//{
//
//	ptMouse.x = ((float)ptMouse.x + ((float)cameraPos.x - WINCX / 2));
//	ptMouse.y = ((float)ptMouse.y + ((float)cameraPos.y - WINCY / 2));
//
//	m_fAttackAngle = (-1.f) * atan2(ptMouse.y - (m_tInfo.fY + (m_tInfo.fCY * 0.5)), ptMouse.x - m_tInfo.fX);
//}