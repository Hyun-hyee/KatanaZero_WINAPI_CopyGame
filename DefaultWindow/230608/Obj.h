#pragma once

#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Functor.h"

class CObj abstract
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void		Initialize(void)		PURE;
	virtual void			Update(void)		PURE;
	virtual void		LateUpdate(void)		PURE;
	virtual void		Render(HDC hDC)			PURE;
	virtual void		Release(void)			PURE;
	virtual void		InitImage()				PURE;

public:
	virtual	int			OnCollision(CObj* _target, DIR _dir)		PURE;

protected:
	void		Update_Rect();

protected:
	OBJ_TYPE		m_Type;
	INFO			m_tInfo;
	
	RECT			m_tRect;
	RECT			m_Collide;
	COLLIDER_TYPE	m_Collider_type;

	float			m_fSpeed;
	float			m_fAccel;
	float			m_fSpeed_Vertical;
	float			m_fAccelTime;
	
	OBJ_STATE		m_State;
	OBJ_STATE		m_PrevState;
	
	CObj*			m_Owner;

public:
	//오너 설정
	void		SetOwner(CObj* _owner) { m_Owner = _owner; }
	CObj*		GetOwner()			   { return m_Owner; }
	
	//속도,가속도 설정
	void		SetAccel(float _accel) { m_fAccel = _accel; }
	void		SetSpeed(float _speed) { m_fSpeed = _speed; }

	//중심점+크기 정보
	INFO*		Get_Info() { return &m_tInfo; }
	void		Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
	void		Set_Size(float _fCX, float _fCY) { m_tInfo.fCX = _fCX; m_tInfo.fCY = _fCY; }

	//OBJ 타입
	OBJ_TYPE	Get_Type() { return m_Type; }
	
	//OBJ 상태
	void		Set_State(OBJ_STATE _state) { m_State = _state; }
	OBJ_STATE	Get_State() { return m_State; }
	
	//Collider
	COLLIDER_TYPE Get_Collider_type() { return m_Collider_type; }
	RECT*		Get_Collide() { return &m_Collide; }





	//Anim KEY,Frame 
protected:
	map<OBJ_STATE,FRAME>	m_FrameMap;

protected:
	void RatioFixByImage(const TCHAR* _tcAnimKey);

	void BasicRender(HDC hDC);
	void FrameRender(HDC hDC);
	void TempRender(HDC hDC);

	void Move_Frame();







	//FrontAngle,FrontCollide
protected:
	float				m_fFrontAngle;
	RECT				m_FrontCollide;
	float				m_FrontCWidth;

public:
	void  Set_FrontAngle(float _angle) { m_fFrontAngle = _angle; }
	float Get_FrontAngle() { return m_fFrontAngle; }

	void  Set_FrontCollide(float _left, float _top, float _right, float _bottom) //L,T,R,B
	{
		m_FrontCollide.left = _left;
		m_FrontCollide.top = _top;
		m_FrontCollide.right = _right;
		m_FrontCollide.bottom = _bottom;
	}
	void  Set_FrontCollide(RECT _collide) { m_FrontCollide = _collide; }


	RECT Get_FrontCollide() { return m_FrontCollide; }

	void Set_FrontWidth(float _width) { m_FrontCWidth = _width; }


	void		SetfX(float _fX) { m_tInfo.fX = _fX; }
};

