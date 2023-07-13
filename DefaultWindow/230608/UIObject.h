#pragma once
#include "Obj.h"
class CUIObject :
    public CObj
{
public:
	CUIObject();
	virtual ~CUIObject();

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
	float		m_FirstX;
	float		m_FirstY;
public :
	void		Set_FirstX(float _x) { m_FirstX = _x; }
	void		Set_FirstY(float _y) { m_FirstY = _y; }
};

