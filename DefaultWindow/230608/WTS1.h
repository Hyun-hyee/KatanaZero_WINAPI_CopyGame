#pragma once
#include "Scene.h"
class CWTS1 :
    public CScene
{
public:
    CWTS1();
    virtual ~CWTS1();
public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC _hDC) override;
    virtual void Release() override;
private:

};

