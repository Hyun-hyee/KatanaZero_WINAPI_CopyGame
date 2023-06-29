#pragma once
#include "Skill.h"
class CFollowBullet :
	public CSkill
{
public:

	CFollowBullet();
	virtual ~CFollowBullet();

	virtual void Initialize(void)		override;
	virtual void	 Update(void)			override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;

protected:
	CObj* m_target;
	ULONGLONG m_fDelay;
	bool	m_bUdateAngel;
	float	m_fTime;
	OBJ_TYPE m_OwnerType;

public:
	void SetAngle(INFO* _Info);
	void SetAngle(float _angle) { m_fAngle = _angle; };

public:
	void SelectTarget();
	void SetOwnerImage();

	void Attack(CObj* _target);


};

