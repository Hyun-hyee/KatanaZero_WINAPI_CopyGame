#pragma once
#include "Scene.h"
class CSecondStage :
    public CScene
{
public:
	CSecondStage();
	virtual ~CSecondStage();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

};

