#pragma once
#include "Skill.h"
class CInstallAttack :
	public CSkill
{
public:
	CInstallAttack();
	virtual ~CInstallAttack();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

public:
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;

	void		 SetOwnerImage();

	void		 Attack(CObj* _target);

private:
	bool	CheckAttack;
	int		m_iLoop;
};

