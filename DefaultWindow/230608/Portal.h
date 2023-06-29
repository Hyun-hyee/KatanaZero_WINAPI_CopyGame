#pragma once
#include "Obj.h"

class CScene;

class CPortal :
	public CObj
{
public:
	CPortal();
	virtual ~CPortal();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)	override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

	virtual	int		OnCollision(CObj* _target, DIR _dir)		override;



private:
	PORTAL_TYPE m_PortalType;

public:
	void Set_PortalType(PORTAL_TYPE _type) { m_PortalType = _type; }

};

