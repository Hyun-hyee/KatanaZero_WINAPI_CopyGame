#pragma once
#include "Obj.h"
class CItem :
    public CObj
{
public:
	CItem();
	virtual ~CItem();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual void InitImage()			override;

public:
	virtual	int	 InCollision(CObj* _target, DIR _dir)	override;
	virtual	int	 OutCollision(CObj* _target)	override;
	virtual	int	 OnCollision(CObj* _target)					override;

private:
	bool		 bThrow;
	ITEM_TYPE			m_ItemType;
public:
	bool		 GetThrow() { return bThrow; }
	void		 SetThrow(bool _check) { bThrow = _check; SetCollideSize(5.f/2.f);}
	void		 SetITemType(ITEM_TYPE _type);
	ITEM_TYPE    GetITemType() { return m_ItemType; }

};

