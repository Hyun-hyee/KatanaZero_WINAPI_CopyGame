#pragma once
#include "Character.h"
class CBossHead :
    public CCharacter
{
public:
	CBossHead();
	virtual ~CBossHead();

	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual void InitImage()			override;

public:
	virtual	int		InCollision(CObj* _target, DIR _dir)	override;
	virtual	int		OutCollision(CObj* _target)	override;
	virtual	int		OnCollision(CObj* _target, DIR _dir)					override;

public:
	void StateUpdate();
	void Jump();
};

