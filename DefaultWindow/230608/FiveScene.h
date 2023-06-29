#pragma once
#include "Scene.h"

class CFiveScene :
	public CScene
{
public:
	CFiveScene();
	virtual ~CFiveScene();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();



};

