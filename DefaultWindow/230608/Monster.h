#pragma once
#include "Obj.h"
#include "Character.h"


class CMonster  : public CCharacter
{
private:

public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;
	virtual void InitImage()			override;

public:
	void Attack();
	virtual	int			InCollision(CObj* _target, DIR _dir)		override;
	virtual	int		OutCollision(CObj* _target)		override;
	virtual	int		OnCollision(CObj* _target)					override;

};

