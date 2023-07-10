#pragma once
#include "Scene.h"
class CFirstStage :
    public CScene
{
public:
	CFirstStage();
	virtual ~CFirstStage();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

	void		ReplayAndNext();

};

