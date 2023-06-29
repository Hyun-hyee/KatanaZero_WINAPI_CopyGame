#pragma once
#include "Obj.h"
#include "Character.h"

class CPlayer :
    public CCharacter
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void	Initialize(void)	override;
	virtual void		Update(void)	override;
	virtual void	LateUpdate(void)	override;
	virtual void	Render(HDC hDC)		override;
	virtual void	Release(void)		override;

	virtual	int		OnCollision(CObj* _target, DIR _dir)		override ;
	 
private:
	void		Key_Input(void);
	void		Set_Angle();

	void		Jump();

	void		StateUpdate();

private:
	bool				EDITMODE;

public:
	void		FindFrontMonster(vector<CObj*>& _targetVec);
	void		BasicAttack();
	void		InstallAttack();
	void		LaserAttack();
	void		SharkAttack();
	void		FollowBullet();


	CObj* Pet;
};

