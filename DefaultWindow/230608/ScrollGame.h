
#pragma once

#include "Scene.h"

class CScrollGame : public CScene
{
	public:
		CScrollGame();
		~CScrollGame();

	public:
		void		Initialize();
		void		Update();
		void		LateUpdate();
		void		Render(HDC _hDC);
		void		Release();



};

