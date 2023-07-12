#pragma once

#define		WINCX		1344
#define		WINCY		784
#define		PURE		= 0
#define		PI			3.141592f
#define		G			9.8f

#define		OBJ_NOEVENT	 0
#define		OBJ_DEAD	 1
#define		VK_MAX		0xff

#define		SMALL		 4/5
#define		SPEED_LIMIT	 6.f

#define		ORIGIN_FRAME	10

extern HWND		g_hWnd;
extern DWORD	g_FrameLimit;
extern bool		g_SlowMotion;
extern DWORD	g_SlowJumpTime;
extern float	g_fSound;
extern bool		g_BossStart;
extern bool		g_BossDead;
extern bool		g_BossPhaseOff;
extern int		g_ReverseOn;
extern bool		g_CollideCheck;
extern bool		g_ClearReverse;
extern bool     g_TimeStop;
extern bool     g_FourRender;

#define		SOUND_VOL1	 1.f
#define		SOUND_VOL2	 0.9f
#define		SOUND_VOL3	 0.6f
#define		SOUND_VOL4	 0.4f


enum COLOR
{
	DEFAULT,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAJENTA,
	MAJENTA1,
	MAJENTA2,
	MAJENTA3,
	MAJENTA4,
	MAJENTA5,
	GRAY,
	COLOR_END
};

static TCHAR* COLORKEY[COLOR_END]
{
	L"",
	L"_R",
	L"_G",
	L"_B",
	L"_Y",
	L"_M",
	L"_M1",
	L"_M2",
	L"_M3",
	L"_M4",
	L"_M5",
	L"_M1",
};


extern COLOR	g_ColorOverlay;