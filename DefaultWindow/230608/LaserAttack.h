#pragma once
#include "Skill.h"
class CLaserAttack : public CSkill
{
public:
	CLaserAttack();
	virtual ~CLaserAttack();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

public:
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;

	void		 SetOwnerImage();
};

