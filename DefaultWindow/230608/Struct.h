#pragma once

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


/******************************Frame******************************/
typedef struct tagFrame
{
	TCHAR*		AnimKey;
	int			iFrameStart;
	int			iFrameEnd;
	int			iMotion; //세로 줄

	int			iFrameSizeX;
	int			iFrameSizeY;

	DWORD		dwSpeed;
	DWORD		dwTime;

}FRAME;
