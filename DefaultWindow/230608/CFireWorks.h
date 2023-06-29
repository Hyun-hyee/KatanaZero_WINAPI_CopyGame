#pragma once
#include "Monster.h"
class CFireWorks :
    public CMonster
{public:
	CFireWorks();
	~CFireWorks();
public:
	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;
public:

private:
};

