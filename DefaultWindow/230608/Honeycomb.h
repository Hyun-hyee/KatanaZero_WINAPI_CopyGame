#pragma once
#include "Monster.h"
class CHoneycomb :
    public CMonster
{
public:
	CHoneycomb();
	virtual ~CHoneycomb();
public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;
	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;
public:
	void Create_Bee();
	void Gravity();

	void StateUpdate();
private:
	list<CObj*> m_BeeList;

	DWORD m_dwTime;
};

