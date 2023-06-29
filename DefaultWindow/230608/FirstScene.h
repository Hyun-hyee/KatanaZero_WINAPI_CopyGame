#pragma once
#include "Scene.h"

class CFirstScene :
	public CScene
{
public:
	CFirstScene();
	virtual ~CFirstScene();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();



};

