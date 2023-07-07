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

	void			Update_CheckCollide();
	void			SetCheckCWidth(float _width) { m_CheckCWidth = _width; }
private:
	CObj*			m_Target;
	RECT			m_CheckCollide;
	float			m_CheckCWidth;
	bool			m_bFollow;
	bool			m_HurtOn;
	bool			m_AttackOn;
	bool			m_BulletOn;
	DWORD			m_WalkTime;
	DWORD			m_AttackTime;
	DWORD			m_SlowTime;
	bool			m_BulletHurt;

public:
	bool			CheckTargetFront();
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
	CObj*			m_Laser;
	int				m_PrevFrame;

	bool			m_Laser90;
};

