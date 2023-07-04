#pragma once
#include "Obj.h"

template <typename T>
class CObjFactory
{
public :
	CObjFactory() {}
	~CObjFactory() {}

	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float _fX, float _fY, float _fCX, float _fCY )
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX,_fY);
		pObj->Set_Size(_fCX, _fCY);
		pObj->Initialize();
		return pObj;
	}

	static CObj* CreateRECT(float _left, float _top, float _right, float _bottom)
	{
		float fCX = _right - _left;
		float fCY = _bottom - _top;
		float fX = _left + 0.5 * fCX;
		float fY = _top + 0.5 * fCY;

		CObj* pObj = new T;
		pObj->Set_Pos(fX, fY);
		pObj->Set_Size(fCX, fCY);
		pObj->Initialize();
		return pObj;
	}


};