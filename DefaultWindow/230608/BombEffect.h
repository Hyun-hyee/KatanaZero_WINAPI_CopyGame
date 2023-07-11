#pragma once
#include "Obj.h"

class CBombEffect :
	public CObj
{
public:
	CBombEffect();
	virtual ~CBombEffect();

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
	float		m_DistanceX;
	float		m_DistanceY;

	DWORD		m_EffectTime;

public:
	void		Set_Distance(float _x, float _y) { m_DistanceX = _x; m_DistanceY = _y; }
};

