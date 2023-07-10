#pragma once
#include "Obj.h"
class CLaserObject :
    public CObj
{
public:
	CLaserObject();
	virtual ~CLaserObject();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual void InitImage()			override;

public:
	virtual	int	 InCollision(CObj* _target, DIR _dir)	override;
	virtual	int	 OutCollision(CObj* _target)	override;
	virtual	int	 OnCollision(CObj* _target, DIR _dir)					override;

private:
	bool		 m_AttackOn;
public:
	bool		GetAttackOn() { return m_AttackOn; }
	void		SetHeight(float _num) { m_tInfo.fCY = _num; }

};

