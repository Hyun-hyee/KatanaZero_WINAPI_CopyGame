#pragma once
#include "Obj.h"
#include "Character.h"


class CMonster  : public CCharacter
{
private:
	list<CObj*>* m_pBulletList;

public:
	CMonster();
	virtual ~CMonster();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)		override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

protected:

public:
	void Attack();
	virtual	int			OnCollision(CObj* _target, DIR _dir)		override;

};

