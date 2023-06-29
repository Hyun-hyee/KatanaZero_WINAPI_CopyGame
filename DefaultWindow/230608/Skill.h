#pragma once
#include "Obj.h"
class CSkill :
    public CObj
{
public:
	CSkill();
	virtual ~CSkill();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;

public:
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;

protected:
	float	m_fAngle;

	void SetAngle(float _angle) { m_fAngle = _angle; }
};

