#include "stdafx.h"
#include "Bullet.h"

CBullet::CBullet() : m_AttackAngle(0), m_Life(3), m_DirX(1), m_DirY(1)
{
	m_Type = SKILL;
	m_Collider_type = ELLIPSE;
}

CBullet::~CBullet()
{
}

void CBullet::Initialize(void)
{
	m_fSpeed = 6.f;
}

void CBullet::Update(void)
{
	if (m_dir != DIR_NONE)
	{
		switch (m_dir)
		{
		case LEFT:
			m_tInfo.fX -= m_fSpeed;
			break;

		case RIGHT:
			m_tInfo.fX += m_fSpeed;
			break;

		case TOP:
			m_tInfo.fY -= m_fSpeed;
			break;

		case BOTTOM:
			m_tInfo.fY += m_fSpeed;
			break;

		}
	}
	else
	{
  			m_tInfo.fX += m_fSpeed * cosf(m_AttackAngle) * m_DirX;
			m_tInfo.fY -= m_fSpeed * sinf(m_AttackAngle) * m_DirY;
	}

	__super::Update_Rect();

}

void CBullet::LateUpdate(void)
{
}

void CBullet::Render(HDC hDC)
{
	Ellipse(hDC,
		m_tRect.left,
		m_tRect.top,
		m_tRect.right,
		m_tRect.bottom );
}

void CBullet::Release(void)
{
}

int CBullet::OnCollision(CObj* _target, DIR _dir)
{
	return OBJ_NOEVENT;
}

void CBullet::SetAngle(INFO* _Info)
{
		m_AttackAngle = (float)atan2((m_tInfo.fY - _Info->fY) , (_Info->fX - m_tInfo.fX));
}




