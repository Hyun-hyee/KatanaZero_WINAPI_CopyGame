#pragma once
#include "Monster.h"
class CShootingStar :
    public CMonster
{
public:
	CShootingStar();
	~CShootingStar();
public:
	virtual void Initialize()			override;
	virtual void Update()				override;
	virtual void LateUpdate(void)		override;
	virtual void Render(HDC hdc)		override;
	virtual void Release(void)			override;
	virtual	int	 OnCollision(CObj* _target, DIR _dir)	override;
public:
	void Set_TargetPos(float _fx,float _fy){
		m_TargetPos.x = _fx;
		m_TargetPos.y = _fy;
	}
	void Set_Target(CObj* _p) { m_pTarget = _p; }

	void Set_Time(int _n) { m_iTime = _n; }

	void Set_Count(int _n) { m_Count = _n; }
	
	void Find_Angle();
private:
	DWORD m_dwTime;
	int m_iTime;
	
	fPOINT m_TargetPos;

	CObj* m_pTarget;

private:

	float m_Width;
	float m_Heigh;
	float m_Distance;
	float m_Radian;

private:
	bool m_bCheck;
	int m_Count;
	int m_Count2;

};

