#pragma once
#include "Skill.h"

class CBullet final : public CSkill
{
public:

	CBullet();
	virtual ~CBullet();

	virtual void Initialize(void)		override;
	virtual void	 Update(void)			override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

	virtual	int	 OnCollision(CObj* _target, DIR _dir)		override;

protected:
	DIR			m_dir;
	float	m_AttackAngle;
	int		m_Life;
	int		m_DirX;
	int		m_DirY;

public:
	void		SetDIR(DIR _dir)			{ m_dir = _dir; }
	void		SetAccel(float _accel)		{ m_fAccel = _accel; }

	void SetAngle(INFO* _Info);
	void SetAngle(float _angle) { m_AttackAngle = _angle; }
};

