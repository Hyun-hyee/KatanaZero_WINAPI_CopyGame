#pragma once
#include "Scene.h"
#include "Struct.h"

class CStartMenu :
    public CScene
{
public:
	CStartMenu();
	virtual ~CStartMenu();

public:
	void		Initialize();
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Release();

	void		FrameRender(HDC hDC);
	void		Move_Frame();

private:
	FRAME		m_BackFrame;
};

