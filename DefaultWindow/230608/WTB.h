#pragma once
#include "Monster.h"
class CWTB :
    public CMonster
{
public:
	CWTB();
	virtual ~CWTB();
public:
	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;
public:
	void Pattern();

	void Create_Pattern(WT_STATE _pattern);

	void Attack();

public:
	void Gravity();

	void StateUpdate();
private:
	int m_Life;

private:
	DWORD m_dwTime;

	list<CObj*> m_PatternList;

	CObj* m_pTarget;

	WT_STATE m_WTState;

	int m_Random;

	bool m_bOneTime;
	bool m_Ignore;

	bool m_Start;

	fPOINT m_First;

};

