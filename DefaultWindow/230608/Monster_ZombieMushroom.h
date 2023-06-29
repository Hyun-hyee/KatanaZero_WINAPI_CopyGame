#pragma once
#include "Monster.h"
#include <functional>

class CMonster_ZombieMushroom :
    public CMonster
{
public:
    CMonster_ZombieMushroom();
    virtual ~CMonster_ZombieMushroom();

public:
	virtual void	Initialize(void)	override;
	virtual void	Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;
public:
	void StateUpdate();
	void Gravity();

public:
	enum class STATE
	{
		IDLE, MOVE_TO_ENEMY, MOVE_TO_ANYWHERE, DIE,
		ATTACK_TAIL, ATTACK_ANYWAY, SET_TARGET, JUMP
	};

private:
	STATE	m_eState;
	bool	m_bStateChanged;
	int		m_iPatternRandom;
	map<STATE, function<void(CMonster_ZombieMushroom*)>> m_mFnAction;

public:
	void StateChange(STATE _eState)
	{
		m_eState = _eState;
		m_bStateChanged = true;
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
	void Die();
	void AttackTail();
	void AttackAnyway();
	void SetTarget();
	void Jump();

private:
	CObj* m_pTarget = nullptr;
	ULONGLONG dwStateTimer;
	ULONGLONG dwTailAttackTimer;

private:
	int m_iHP;
};

