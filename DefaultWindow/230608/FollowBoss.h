#pragma once
#include "Monster.h"
#include <functional>

class CFollowBoss :
	public CMonster
{
public:
	CFollowBoss();
	virtual ~CFollowBoss();

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

	/******************************************************************/
public:
	enum class STATE
	{
		IDLE, MOVE_TO_ENEMY, MOVE_TO_ANYWHERE,
		MOVE_TO_LEFT, MOVE_TO_RIGHT, MOVE_TO_CENTER,
		ATTACK_FOLLOWB1,
		ATTACK_FOLLOWB2,
		ATTACK_FOLLOWB3,
		ATTACK_INSTALL1,
		ATTACK_INSTALL2,
		DIE,
		SET_TARGET

	};

private:
	STATE	m_eState;
	bool	m_bStateChanged;
	int		m_iPatternRandom;
	map<STATE, function<void(CFollowBoss*)>> m_mFnAction;

public:
	void StateChange(STATE _eState)
	{
		m_eState = _eState;
		m_bStateChanged = true;
		m_AnimFrameIndex = 0;
	}

	bool IsStateChanged()
	{
		if (m_bStateChanged)
		{
			m_bStateChanged = false;
			return true;
		}
		return m_bStateChanged;
	}

private:
	void Idle();
	void MoveToEnemy();
	void MoveToAnywhere();
	void MoveToRight();
	void MoveToLeft();
	void MoveToCenter();
	void Attack_FollowB1();
	void Attack_FollowB2();
	void Attack_FollowB3();
	void Attack_Install();
	void Attack_Install2();
	void Die();
	void SetTarget();

private:
	CObj* m_pTarget = nullptr;
	ULONGLONG dwStateTimer;
};

