#pragma once
#include "Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent();
	virtual ~CameraComponent() override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	fPOINT BackSize;
public:
	void Set_BackSize(float x, float y) { BackSize.x = x; BackSize.y = y; }

private:
	POINT	PrevMousePos;
	POINT	NowMousePos;
	fPOINT	pos;
};

