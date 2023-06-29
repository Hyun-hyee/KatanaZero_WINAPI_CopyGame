
#pragma once

#include "Scene.h"

class CSecondScene : public CScene
{
public:
	CSecondScene();
	virtual ~CSecondScene();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();


};

