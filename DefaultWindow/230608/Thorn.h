#pragma once
#include "Monster.h"
class CThorn :
    public CMonster
{
public:
	CThorn();
	virtual ~CThorn();
public:
	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;
public:
	void Set_Rect() {
		m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
		m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
		m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
		m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
	}
private:
	DWORD m_dwTime;
	RECT m_Warning;

	bool m_bStart;
};

