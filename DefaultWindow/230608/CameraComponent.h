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

};

