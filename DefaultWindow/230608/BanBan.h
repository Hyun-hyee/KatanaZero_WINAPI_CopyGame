#pragma once
#include "Monster.h"
class CBanBan : public CMonster
{
public:
	CBanBan();
	virtual ~CBanBan();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

public:
	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;

public:
	void				Gravity();
	void				StateUpdate();
	void				BanBan_1();

private:
	float				m_fMoveToNext;
	int					m_iAtk;
	ULONGLONG			dwTime;

private:
	bool	CheckOnLine;
	bool	CheckAttack;
	int		m_iHP;
};

