#pragma once
#include "Monster.h"
class CBee :
    public CMonster
{
public:
	CBee();
	virtual ~CBee();
public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;
	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;
public:
	void StateUpdate();
private:
	CObj* pTarget;
};

