#pragma once
#include "stdafx.h"
#include "Monster.h"

class CWatcherLight :public CMonster
{
public:
	CWatcherLight();
	virtual ~CWatcherLight();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

public:
	void Attack();
	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;

public:
	void				Gravity();
	void				StateUpdate();
	void				SharkAttack();
private:

	float				m_fMoveToNext;
	int				m_bAtk;
	ULONGLONG			dwTime;

private:
	bool	CheckOnLine;
	bool	CheckAttack;
	int		m_iHP;
};

