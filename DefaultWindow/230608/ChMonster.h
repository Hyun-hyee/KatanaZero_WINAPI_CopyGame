#pragma once
#include "Monster.h"
class CChMonster :
	public CMonster
{
public:
	CChMonster();
	virtual ~CChMonster();
public:
	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;
public:
	void Gravity();

	void		StateUpdate();
public:
	void Set_Move_Range(float _LX, float _RX) {
		m_Move_Range_LX = _LX;
		m_Move_Range_RX = _RX;
	}
private:
	/*UINT				m_Count;
	ULONGLONG			dwTime;*/

	/*LONG				m_ChangeLeft;
	LONG				m_ChangeRight;*/

	float				m_Move_Range_LX;
	float				m_Move_Range_RX;

	CObj* pTarget;

};

