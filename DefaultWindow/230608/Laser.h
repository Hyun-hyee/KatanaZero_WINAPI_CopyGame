#pragma once
#include "Obj.h"
class CLaser :
    public CObj
{
public:
	CLaser();
	virtual ~CLaser();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual void InitImage()			override;

public:
	virtual	int		InCollision(CObj* _target, DIR _dir)	override;
	virtual	int		OutCollision(CObj* _target)	override;
	virtual	int		OnCollision(CObj* _target, DIR _dir)					override;

public:
	void StateUpdate();

private:
	DWORD  m_ChangeTime;
	int	   m_OnTime;
	LASERTYPE m_LaserType;
	fPOINT m_Axis;
	bool	m_LaserAttack;

public:
	void	SetLaserAttack(bool _on) { m_LaserAttack = _on; }
	void	SetOnTime(int _num) { m_OnTime = _num; }
	void	SetLaserType(LASERTYPE _type) { m_LaserType = _type; }
	void	SetAxis(float _x, float _y) { m_Axis.x = _x; m_Axis.y = _y; }
	void	Update_AttackAngle();

	bool	GetLaserAttack() { return m_LaserAttack; }
	void	Update_Info();

	void	RotateAxis(float _Addx, float _y);
};

