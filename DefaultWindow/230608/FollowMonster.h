#pragma once
#include "Monster.h"

class CFollowMonster :
	public CMonster
{
public:
	CFollowMonster();
	virtual ~CFollowMonster();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

protected:

public:
	void Attack();
	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;

public:
	void Gravity();
	void StateUpdate();

	void	InstallAttack();
	void	FollowBullet();

private:
	bool	CheckOnLine;
	bool	CheckAttack;
	int		m_Life;

public:
	void	Set_Life(int _life) { m_Life = _life; }
	int		Get_Life() { return m_Life; }
	void	Cal_Life(int _life) { m_Life += _life; }


};

