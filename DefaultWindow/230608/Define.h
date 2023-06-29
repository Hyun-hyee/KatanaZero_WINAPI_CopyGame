#pragma once

#define		WINCX		800
#define		WINCY		600
#define		PURE		= 0
#define		PI			3.141592f
#define		G			9.8f

#define		OBJ_NOEVENT	 0
#define		OBJ_DEAD	 1
#define		VK_MAX		0xff

#define		SMALL		 4/5
extern HWND g_hWnd;


/******************************ENUM/STRUCT******************************/
//float POINT
struct fPOINT
{
	float x;
	float y;
	fPOINT() {}
	fPOINT(float x, float y) :x(x), y(y) {}
};
//중점, 가로, 세로 길이
typedef struct tagInfo
{
	float	fX;				// 중점 X
	float	fY;				// 중점 Y
	float	fCX;			// 가로 길이
	float	fCY;			// 세로 길이

	float	fPrevX = 0.f;	// 이전 X
	float	fPrevY = 0.f;	// 이전 Y
	float	fRatioX = 1.f;	// 비율 X
	float	fRatioY = 1.f;	// 비율 Y
}INFO, POINT_RECT;

//방향
typedef enum Direction
{
	DIR_NONE,
	LEFT = 1,
	RIGHT = 2,
	TOP = 3,
	BOTTOM = 4,

	DIR_END
}DIR;

//Obj 타입
typedef enum OBJID
{
	OBJ_NONE,
	PLAYER,
	SKILL,
	EFFECT,
	MONSTER,
	SHIELD,
	GROUND,
	PORTAL,
	OBJ_TYPE_END
}OBJ_TYPE;

//Obj 상태
typedef enum OBJSTATE
{
	IDLE,
	MOVE,
	ATTACK,
	BEFORE_DEAD,
	DEAD,

	OBJ_STATE_END

}OBJ_STATE;

//충돌체 타입
typedef enum COLLIDERTYPE
{
	RECTANGLE,
	ELLIPSE,

	COLLIDER_TYPE_END
}COLLIDER_TYPE;

typedef enum PORTALTYPE
{
	PREV,
	NEXT,

	PORTAL_TYPE_END
}PORTAL_TYPE;

// 우택 보스 패턴
typedef enum WTSTATE {
	WT_BEFORE,
	THORNY_VINE,
	SHOOTING_STAR,
	FIREWORKS,
	WT_IDLE,
	WT_ATTACK,
	WT_MOVE,
	BE_DEAD,
	CHARACTER_STATE_END

}WT_STATE;

//
//static const vector <POINT_RECT>	DISABLE_AREA{
//	{WINCX / 2, 50 / 2, WINCX, 50},
//	{50 / 2, WINCY / 2 , 50, WINCY},
//	{WINCX - 50 / 2, WINCY / 2, 50, WINCY},
//	{WINCX / 2, WINCY - 50 / 2, WINCX,50} };

//
//static const RECT					PLAY_AREA{ 50,50,WINCX - 50 ,WINCY - 50 };


/******************************DELETE******************************/
template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

class CDeleteObj
{
public:
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void operator()(T& Pair)
	{
		if (Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};



/******************************LINE******************************/
// Line용 float Point
typedef	struct tagLinePoint
{
	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

	float	fX;
	float	fY;

}LINEPOINT;

//Line 구조체 (왼쪽 점, 오른쪽 점)
typedef struct tagLine
{
	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& _tLpoint, LINEPOINT& _tRpoint)
		: tLpoint(_tLpoint), tRpoint(_tRpoint) { }

	LINEPOINT	tLpoint;
	LINEPOINT	tRpoint;

}LINE;


/******************************FUNCTOR******************************/
class CTag_Finder
{
public:
	CTag_Finder(const TCHAR* pString) : m_pString(pString) {}
public:
	template<typename T>
	bool operator()(T& Pair)
	{
		return !lstrcmp(m_pString, Pair.first);
	}

private:
	const TCHAR* m_pString;
};
