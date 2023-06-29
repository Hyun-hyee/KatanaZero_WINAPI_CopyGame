#include "stdafx.h"
#include "ShootingStar.h"
#include "BmpMgr.h"
#include "SceneManager.h"
#include "ObjMgr.h"

CShootingStar::CShootingStar():m_dwTime(GetTickCount64()), m_iTime(0), m_bCheck(false), m_Count(0), m_Count2(0)
{
    ZeroMemory(&m_TargetPos, sizeof(m_TargetPos));
}

CShootingStar::~CShootingStar()
{
    Release();
}

void CShootingStar::Initialize()
{
    m_fSpeed = 25.f;

    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/우택/보스 패턴/투사체/투사체2.gif", L"Shoot_IDLE");

    m_AnimMap.insert({ L"Shoot_IDLE",0 });

    AnimChange(L"Shoot_IDLE");
    RatioFixByImage(m_AnimKey);

    //m_pTarget = CObjMgr::Get_Instance()->Get_Player();
}

void CShootingStar::Update()
{
    m_Count2++;
    //if (m_dwTime + m_iTime - 100 < GetTickCount64()&&m_bCheck) {

    //    /*m_TargetPos.x = CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fX;
    //    m_TargetPos.y = CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fY;*/
    //    m_TargetPos.x = m_pTarget->Get_Info()->fX;
    //    m_TargetPos.y = m_pTarget->Get_Info()->fY;

    //    m_Width = m_TargetPos.x - m_tInfo.fX;
    //    m_Heigh = m_TargetPos.y - m_tInfo.fY;

    //    m_Distance = sqrtf(m_Heigh * m_Heigh + m_Width * m_Width);

    //    m_Radian = acosf(m_Width / m_Distance);

    //    /*if (m_TargetPos.y > m_tInfo.fY) {
    //        m_Radian = (2 * PI) - m_Radian;
    //    }*/

    //    m_dwTime = GetTickCount64();
    //    m_bCheck = false;
    //}
    if (m_Count2 > m_Count&&!m_bCheck) {
        m_Width = m_pTarget->Get_Info()->fX - m_tInfo.fX;
        m_Heigh = m_pTarget->Get_Info()->fY - m_tInfo.fY;

        m_Distance = sqrtf(m_Heigh * m_Heigh + m_Width * m_Width);

        m_Radian = acosf(m_Width / m_Distance);

        /*if (m_TargetPos.y > m_tInfo.fY) {
            m_Radian = (2 * PI) - m_Radian;
        }*/
        m_bCheck = true;
        m_Count2 = 0;
    }
    else if(m_Count2 > m_Count) {
        m_State = DEAD;
    }

    if (m_bCheck) {
        m_tInfo.fX += cosf(m_Radian) * m_fSpeed;
        m_tInfo.fY += sinf(m_Radian) * m_fSpeed;
    }

    CObj::Update_Rect();
}

void CShootingStar::LateUpdate(void)
{
    /*if (m_tRect.bottom >= 900.f) {
        m_State = DEAD;
    }*/
    /*if (m_Count2 > m_Count && m_bCheck) {
        m_State = DEAD;
    }*/
}

void CShootingStar::Render(HDC hdc)
{

    CObj::Render(hdc);
    
}

void CShootingStar::Release(void)
{
}

int CShootingStar::OnCollision(CObj* _target, DIR _dir)
{
    return 0;
}

void CShootingStar::Find_Angle()
{
    if (m_dwTime + m_iTime - 100 < GetTickCount64()) {

        /*m_TargetPos.x = CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fX;
        m_TargetPos.y = CObjMgr::Get_Instance()->Get_Player()->Get_Info()->fY;*/
        m_TargetPos.x = m_pTarget->Get_Info()->fX;
        m_TargetPos.y = m_pTarget->Get_Info()->fY;

        m_Width = m_TargetPos.x - m_tInfo.fX;
        m_Heigh = m_TargetPos.y - m_tInfo.fY;

        m_Distance = sqrtf(m_Heigh * m_Heigh + m_Width * m_Width);

        m_Radian = acosf(m_Width / m_Distance);

        if (m_TargetPos.y > m_tInfo.fY) {
            m_Radian = (2 * PI) - m_Radian;
        }

        m_dwTime = GetTickCount64();
    }
}
