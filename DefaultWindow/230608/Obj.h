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
	
	//CObj& operator = (const CObj& other);

public:
	virtual void		Initialize(void)		PURE;
	virtual void			Update(void)		PURE;
	virtual void		LateUpdate(void)		PURE;
	virtual void		Render(HDC hDC)			PURE;
	virtual void		Release(void)			PURE;
	virtual void		InitImage()				PURE;

public:
	virtual	int			InCollision(CObj* _target, DIR _dir)		PURE;
	virtual	int			OutCollision(CObj* _target)					PURE;
	virtual	int			OnCollision(CObj* _target, DIR _dir)					PURE;

protected:
	void		Update_Rect();

protected:
	//����
	OBJ_TYPE		m_Type;
	INFO			m_tInfo;
	
	//�ٵ� ũ��, �⺻ �ٵ� �ݶ��̴�
	RECT			m_tRect;
	RECT			m_Collide;
	COLLIDER_TYPE	m_Collider_type;
	float			m_CollideSize;
public:
	void			SetCollideSize(float _size) { m_CollideSize = _size; }

protected:
	// �ӵ�, ���ӵ�, Vertical Speed
	float			m_fSpeed;
	float			m_fAccel;
	float			m_fSpeed_Vertical;
	
	//����, ���� ����
	OBJ_STATE		m_State;
	OBJ_STATE		m_PrevState;
	
	// ���� 
	CObj*			m_Owner;

public:
	//���� ����
	void		SetOwner(CObj* _owner) { m_Owner = _owner; }
	CObj*		GetOwner()			   { return m_Owner; }
	
	//�ӵ�,���ӵ� ����
	void		SetAccel(float _accel) { m_fAccel = _accel; }
	void		SetSpeed(float _speed) { m_fSpeed = _speed; }

	//�߽���+ũ�� ���� ����
	INFO*		Get_Info() { return &m_tInfo; }
	void		Set_Pos(float _fX, float _fY) { m_tInfo.fX = _fX; m_tInfo.fY = _fY; }
	void		Set_Size(float _fCX, float _fCY) { m_tInfo.fCX = _fCX; m_tInfo.fCY = _fCY; }

	//OBJ Ÿ�� ����
	OBJ_TYPE	Get_Type() { return m_Type; }
	
	//OBJ ���� ����
	void		Set_State(OBJ_STATE _state) { m_State = _state; }
	OBJ_STATE	Get_State() { return m_State; }
	
	//Collider ����
	COLLIDER_TYPE Get_Collider_type() { return m_Collider_type; }
	RECT*		Get_Collide() { return &m_Collide; }
	RECT*		Get_Rect() { return &m_tRect; }




	//Anim KEY,Frame ����
protected:
	map<OBJ_STATE,FRAME>	m_FrameMap;
	TCHAR* m_OneImgKey;
	bool   m_FrameReverse;

protected:
	void RatioFixByImage(const TCHAR* _tcAnimKey);

	void BasicRender(HDC hDC);
	void FrameRender(HDC hDC);
	void FrameRender_OriginSize(HDC hDC);
	void FrameRender_OriginSize(HDC hDC, int _y); //������ ������Ʈ��
	void RotateRender(HDC hDC, float _angle);
	void RotateFrameRender(HDC hDC, float _angle, float _resizeX, float _resizeY); 
	void RotateFrameRender_Vertical(HDC hDC, float _angle, float _resizeX, float _resizeY);
	void CollideRender(HDC hDC);
	void CollideRender(HDC hDC, RECT _collide);
	void FrameRenderToBlackWhite(HDC hDC);
	
	void Move_Frame();



	//FrontAngle,FrontCollide (���� ��ü �ν� �ݶ��̴�, �ٶ󺸴� ����(��))
protected:
	float				m_fFrontAngle;
	RECT				m_FrontCollide;
	float				m_fFrontCWidth;

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
	RECT* Get_FrontCollide() { return &m_FrontCollide; }

	//FrontCollide ���� ����
	void Set_FrontWidth(float _width) { m_fFrontCWidth = _width; }

	//X ��ǥ ��ġ ����
	void		SetfX(float _fX) { m_tInfo.fX = _fX; }

	
	//AttackAngle, AttackCollide (���� �ݶ��̴�, �����ϴ� ����(��))
	//*** OBB�� ����� ���� �ʿ� ***//
protected:
		float				m_fAttackAngle;
		RECT				m_AttackCollide;
		INFO				m_AttackCInfo;
		float				m_AttackCDistance;
public:
		void  Set_AttackCollide(float _left, float _top, float _right, float _bottom) //L,T,R,B
		{
			m_AttackCollide.left = _left;
			m_AttackCollide.top = _top;
			m_AttackCollide.right = _right;
			m_AttackCollide.bottom = _bottom;
		}
		void Set_AttackAngle(float _angle) { m_fAttackAngle = _angle; }
		float Get_AttackAngle() { return m_fAttackAngle; }
		void  Set_AttackCollide(RECT _collide) { m_FrontCollide = _collide; }
		RECT* Get_AttackCollide() { return &m_FrontCollide; }
		void UpdateAttackCollide();

public:
	void SlowMotionUpdate();

	void ConvertToBlackWhite(Gdiplus:: Bitmap* bitmap);
	void ConvertToGrayScale(Gdiplus::Bitmap* bitmap);
	Gdiplus::Bitmap* CloneBitmap(Gdiplus::Bitmap* sourceBitmap);

protected:
	//������
	bool		 bThrow;
	ITEM_TYPE	 m_ItemType;

public :
	void		Set_FrameStart(OBJSTATE _state, int _frame) { m_FrameMap[_state].iFrameStart = _frame; }

};

