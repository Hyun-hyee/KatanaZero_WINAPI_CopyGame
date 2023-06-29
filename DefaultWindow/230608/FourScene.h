
#pragma once

#include "Scene.h"

class CFourScene : public CScene
{
public:
	CFourScene();
	virtual ~CFourScene();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();


};

