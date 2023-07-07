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
	virtual void	InitImage()			override;

	virtual	int		InCollision(CObj* _target, DIR _dir)		override;
	virtual	int		OutCollision(CObj* _target)					override;
	virtual	int		OnCollision(CObj* _target, DIR _dir)					override;
private:
	void		Key_Input(void);

	void		Jump();

	void		StateUpdate();
	void		AttackAngleUpdate();

	void		UseItem();

private:
	bool				EDITMODE;
	bool				CAMERAMODE;

	bool				m_WallJump;
	float				m_fWallSpeed;
	float				m_fFixAttackAngle;


	DWORD				m_RunningSoundTime;
	int					m_SlowRun;
	int					m_PlayerSoundCh;
					

public:
	void				PlayerPlaySound(TCHAR* _name);
	void				AttackSound();

	void				BatteryChange();

	void				Attack();
	void				Parring();

private:

	ITEM_TYPE			m_ItemState;
	int					m_BatteryCount;
	DWORD				m_BatteryTime;
	bool				m_HurtOn;

public:
	ITEM_TYPE			GetItemState() { return m_ItemState; }
	int					GetBatteryCount() { return m_BatteryCount; }
	void				CameraReMake();
};

