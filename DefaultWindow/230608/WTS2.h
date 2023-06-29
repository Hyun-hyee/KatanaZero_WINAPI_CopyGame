#pragma once
#include "Scene.h"
class CWTS2 :
    public CScene
{
public:
    CWTS2();
    virtual ~CWTS2();
public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;
private:
};

