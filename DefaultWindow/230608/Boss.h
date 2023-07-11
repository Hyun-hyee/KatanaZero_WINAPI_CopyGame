#pragma once
#include "Character.h"

class CObj;


class CBoss :
    public CCharacter
{
public:
	CBoss();
	virtual ~CBoss();

public:
	virtual void	Initialize(void)	override;
	virtual void	Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;
	virtual void	InitImage()			override;

public:
	virtual	int		InCollision(CObj* _target, DIR _dir)		override;
	virtual	int		OutCollision(CObj* _target)		override;
	virtual	int		OnCollision(CObj* _target, DIR _dir)					override;


	void			StateUpdate();

	void			Jump();

private:
	CObj*			m_Target;
	DWORD			m_SlowTime;

public:
	void			SetTarget(CObj* _target) { m_Target = _target; }
	void			Attack();
	void			LaserAttack(LASERTYPE _type,float _x, float _y);

	void Flip_FrontAngle()
	{
		if (m_fFrontAngle == 0)
			m_fFrontAngle = PI;
		else
			m_fFrontAngle = 0;
	}

private:
	DWORD			m_LaserTime;
	DWORD			m_BulletTime; //안쓰면 지우기

	bool			m_WallJump;
	bool			m_Levitation;
	CObj*			m_Laser;
	int				m_PrevFrame;

	bool			m_Laser90;

	int				m_Phase;
	int				m_Life;

	int				m_BossSoundCh;

	typedef void (CBoss ::* pFunction)();

	bool					m_PatternOn;
	vector<pFunction>		m_PatternList;
	int						m_PatternIndex;
	list<CObj*>*			m_EnemyList;

public:
	void			InitPatternList();
	void			PatternChange();
	bool			CheckHurt();

	void			Pattern_GroundLaser(); //조준 레이저 + ROLL + 점프->총 공격 -> 착지 후 은신
	void			Pattern_Laser180(); //
	void			Pattern_LeftLaser90();
	void			Pattern_RightLaser90();
	void			Pattern_LaserBottom_1();
	void			Pattern_LaserBottom_2();
	void			Pattern_LaserBottom_3();
	void			Pattern_LaserBottom_4();
	void			Pattern_Dash();
	void			Pattern_MakeEnemy_1();
	void			Pattern_MakeEnemy_2();
	void			Pattern_MakeEnemy_3();

	void			Pattern_Move_LeftBottom();
	void			Pattern_Move_RightBottom();

	void			Set_Phase(int _phase)
	{
		m_Phase = _phase;
		InitPatternList();
	}
	int				Get_Phase() { return m_Phase; }

	void			Set_Life(int _life) { m_Life = _life;}

	void			StateChangeEffect();
	void			StateChangeSound();

	void			BossPlaySound(TCHAR* _name);

};


