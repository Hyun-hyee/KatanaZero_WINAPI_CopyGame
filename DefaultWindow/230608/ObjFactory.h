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


};