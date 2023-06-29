#include "stdafx.h"
#include "CameraComponent.h"
#include "Character.h"
#include "SceneManager.h"

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Initialize()
{
}

void CameraComponent::Update()
{
	if (_owner == nullptr)
		return;

	fPOINT pos = { _owner->Get_Info()->fX, _owner->Get_Info()->fY };


	// TEMP
	//pos.x = ::clamp(pos.x, 400.f, 3024.f - 400.f);
	//pos.y = ::clamp(pos.y, 300.f, 2064.f - 300.f);
	if (pos.x <= 400.f)
		pos.x = 400.f;
	else if (pos.x >= 1920 - 400.f)
		pos.x = 1920 - 400.f;

	if (pos.y <= 300.f)
		pos.y = 300.f;
	else if (pos.y >= 1080 - 300.f)
		pos.y = 1080 - 300.f;
	CSceneManager::Get_Instance()->SetCameraPos(pos);
}

void CameraComponent::Render(HDC hdc)
{
}
