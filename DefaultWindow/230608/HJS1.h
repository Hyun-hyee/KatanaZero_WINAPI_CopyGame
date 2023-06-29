#pragma once
#include "Scene.h"
class CHJS1 :
    public CScene
{
public:
	CHJS1();
	virtual ~CHJS1();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();


};

