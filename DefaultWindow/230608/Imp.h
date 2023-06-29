#pragma once
#include "Monster.h"
class CImp : public CMonster
{
public:
	CImp();
	virtual ~CImp();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

public:

	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;
	void		StateUpdate();
	void		Gravity();

private:
	float				m_fMoveToNext;
	int					m_iAtk;
	int					iImpHp;
	float				fWidth;
	float				fHeight;
	ULONGLONG			dwTime;

private:
	bool	CheckOnLine;


};

