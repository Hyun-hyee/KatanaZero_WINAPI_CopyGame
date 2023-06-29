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

	BackSize = CSceneManager::Get_Instance()->Get_BackSize();
	fPOINT pos = { _owner->Get_Info()->fX, _owner->Get_Info()->fY };

	if (pos.x <= 400.f)
		pos.x = 400.f;
	else if (pos.x >= BackSize.x - 400.f)
		pos.x = BackSize.x - 400.f;

	if (pos.y <= 300.f)
		pos.y = 300.f;
	else if (pos.y >= BackSize.y - 300.f)
		pos.y = BackSize.y - 300.f;
	CSceneManager::Get_Instance()->SetCameraPos(pos);

}

void CameraComponent::Render(HDC hdc)
{
}
