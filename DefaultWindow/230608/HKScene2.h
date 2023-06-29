#pragma once
#include "Scene.h"
class CHKScene2 :
    public CScene
{
public:
	CHKScene2();
	virtual ~CHKScene2();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();
};

