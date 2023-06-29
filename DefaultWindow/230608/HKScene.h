#pragma once
#include "Scene.h"
class CHKScene :
    public CScene
{
public:
	CHKScene();
	virtual ~CHKScene();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();
};

