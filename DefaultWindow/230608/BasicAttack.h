#pragma once
#include "Skill.h"
class CBasicAttack :
	public CSkill
{
public:
	CBasicAttack();
	virtual ~CBasicAttack();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

public:
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;

	void		SetOwnerImage();
	CObj* Get_Target() { return m_pTarget; }
	void		Set_Target(CObj* _target) { m_pTarget = _target; }
private:
	CObj* m_pTarget;

};

