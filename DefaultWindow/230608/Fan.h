#pragma once
#include "Obj.h"
class CFan :
    public CObj
{
public:
	CFan();
	virtual ~CFan();

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
};

