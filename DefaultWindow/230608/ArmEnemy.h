#pragma once
#include "Obj.h"
#include "Character.h"


class CArmEnemy : public CCharacter
{
private:

public:
	CArmEnemy();
	virtual ~CArmEnemy();

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
	DWORD			m_WalkTime;
	bool			m_BulletHurt;


public:
	bool			CheckTargetFront();
	void			SetTarget(CObj* _target) { m_Target = _target; }
	void			Attack();

	void			StateChangeEffect();
	void			StateChangeSound();
};

