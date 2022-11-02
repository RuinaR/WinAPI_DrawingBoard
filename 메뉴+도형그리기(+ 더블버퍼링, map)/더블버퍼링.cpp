#include <windows.h>		// 윈도우 헤더파일
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <map>
#include "resource.h"
using namespace std;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;										//인스턴스 핸들
LPCTSTR lpszClass = TEXT("최원준_22311019");				//제목 표시줄에 표시되는 내용

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam,
	int nCmdShow)
{
	HWND hWnd;											//윈도우 핸들 선언
	MSG Message;										//메세지 구조체 변수 선언
	WNDCLASS WndClass;									//Windows Class 구조체 변수 선언
	g_hInst = hInstance;								//hInstance값을 외부에서도 사용할 수 있도록 전역변수에 값을 저장

	WndClass.cbClsExtra = 0;							//예약 영역. 지금은 사용X
	WndClass.cbWndExtra = 0;							//예약 영역
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 윈도우의 배경 색상을 지정
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//윈도우의 마우스포인터 모양을 지정
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//윈도우의 커서 아이콘 모양 지정
	WndClass.hInstance = hInstance;						//윈도우 클래스를 등록하는 프로그램 번호
	WndClass.lpfnWndProc = WndProc;						//윈도우 메세지 처리 함수 지정
	WndClass.lpszClassName = lpszClass;					//윈도우 클래스의 이름 지정
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU3);						//메뉴 지정
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//윈도우의 스타일을 정의

	RegisterClass(&WndClass);							//WNDCLASS 구조체의 번지를 전달


	hWnd = CreateWindow(lpszClass, lpszClass,			//윈도우를 생성
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		/* 100,100,500,500*/, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	//메세지 루프를 실행
	while (GetMessage(&Message, NULL, 0, 0)) {			//Queue에 있는 메세지를 읽어들인다
		TranslateMessage(&Message);						//키보드 입력 메세지를 가공하여 프로그램에서 쉽게 
														//  사용할 수 있도록
		DispatchMessage(&Message);						//메세지를 윈도우의 메세지 처리 함수 WndProc로 전달
	}
	return (int)Message.wParam;							//탈출 코드. 프로그램 종료
}

double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my, int r)
{
	if (LengthPts(x, y, mx, my) < r)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

enum SHAPE
{
	NULL_SHAPE = 0,
	LINE_SHAPE,
	CIRCLE_SHAPE,
	RECT_SHAPE
};
enum COLOR
{
	RED,
	GREEN,
	BLUE,
	BLACK,
	WHITE,
	MAX
};
COLORREF arrColor[COLOR::MAX];

void SetData(int* changeData, const int* value);

void SetShape_Line(void);
void SetShape_Circle(void);
void SetShape_Rect(void);
void SetBrush_Red(void);
void SetBrush_Green(void);
void SetBrush_Blue(void);
void SetPen_Red(void);
void SetPen_Green(void);
void SetPen_Blue(void);
void SetLineW_1(void);
void SetLineW_2(void);
void SetLineW_3(void);
void SetLineW_4(void);
void SetLineW_5(void);
void End(void);

SHAPE shape;	//선택한 모양 타입
COLOR brushColor;	//브러시 색
COLOR penColor;	//펜 색
int penW;	//펜 굵기
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static PAINTSTRUCT ps;

	//그리기에 사용할 변수
	static POINT start; //그려질 시작 위치
	static POINT end;	//그려질 끝 위치
	static bool isLbtnDown;	//마우스 클릭 상태 저장하는 변수
	static bool isClear;	//화면 지우기용 변수
	//함수포인터 map
	static map<int, void(*)(void)> funcMap;
	static map<int, void(*)(void)>::iterator iter;
	//더블버퍼링용 변수
	static HDC hdcBuff;
	HBITMAP hBmpBuff;
	HBITMAP hBmpBuffOld;
	RECT clientRect;
	//brush, pen
	static HBRUSH brush;
	static HPEN pen;
	switch (iMessage)
	{
	case WM_CREATE:	//변수 초기화, 기본설정
		shape = SHAPE::NULL_SHAPE;
		brushColor = COLOR::WHITE;
		penColor = COLOR::BLACK;
		penW = 1;
		start = { 0 };
		end = { 0 };
		isLbtnDown = false;
		isClear = false;
		arrColor[COLOR::RED] = RGB(255, 0, 0);
		arrColor[COLOR::GREEN] = RGB(0, 255, 0);
		arrColor[COLOR::BLUE] = RGB(0, 0, 255);
		arrColor[COLOR::BLACK] = RGB(0, 0, 0);
		arrColor[COLOR::WHITE] = RGB(255, 255, 255);
		funcMap.insert(make_pair(M_LINE, SetShape_Line));
		funcMap.insert(make_pair(M_CIRCLE, SetShape_Circle));
		funcMap.insert(make_pair(M_RECT, SetShape_Rect));
		funcMap.insert(make_pair(BRUSH_RED, SetBrush_Red));
		funcMap.insert(make_pair(BRUSH_GREEN, SetBrush_Green));
		funcMap.insert(make_pair(BRUSH_BLUE, SetBrush_Blue));
		funcMap.insert(make_pair(PEN_RED, SetPen_Red));
		funcMap.insert(make_pair(PEN_GREEN, SetPen_Green));
		funcMap.insert(make_pair(PEN_BLUE, SetPen_Blue));
		funcMap.insert(make_pair(LINE_W_1, SetLineW_1));
		funcMap.insert(make_pair(LINE_W_2, SetLineW_2));
		funcMap.insert(make_pair(LINE_W_3, SetLineW_3));
		funcMap.insert(make_pair(LINE_W_4, SetLineW_4));
		funcMap.insert(make_pair(LINE_W_5, SetLineW_5));
		funcMap.insert(make_pair(M_END, End));
		break;
	case WM_COMMAND:
		iter = funcMap.find(LOWORD(wParam));
		if (iter != funcMap.end())
		{
			iter->second();
		}	
		break;
	case WM_LBUTTONDOWN:	//좌클릭 시 
		//그릴 시작 좌표 저장
		isLbtnDown = true;
		start.x = LOWORD(lParam);
		start.y = HIWORD(lParam);
		//화면 클리어
		isClear = true;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		//좌클릭 상태로 움직이면 그려질 끝 좌표를 저장하고 WM_PAINT 실행
		if (isLbtnDown)
		{
			end.x = LOWORD(lParam);
			end.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, false);
		}
		break;
	case WM_LBUTTONUP:
		isLbtnDown = false;
		break;
	case WM_PAINT:
		if (isClear)	//화면 클리어인 경우
		{
			isClear = false;	//false로 바꾸고
			break;				//아무것도 안하고 리턴
		}

		hdc = BeginPaint(hWnd, &ps);

		//더블 버퍼링용 DC, BMP
		GetClientRect(hWnd, &clientRect);
		hdcBuff = CreateCompatibleDC(hdc);
		hBmpBuff = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		hBmpBuffOld = (HBITMAP)SelectObject(hdcBuff, hBmpBuff);
		//PatBlt(hdcBuff, 0, 0, clientRect.right, clientRect.bottom, WHITENESS);
		FillRect(hdcBuff, &clientRect, (HBRUSH)COLOR_WINDOWFRAME);
		//브러시, 펜 생성
		brush = CreateSolidBrush(arrColor[brushColor]);
		pen = CreatePen(PS_SOLID, penW, arrColor[penColor]);
		//브러시, 펜 등록
		SelectObject(hdcBuff, brush);
		SelectObject(hdcBuff, pen);
		switch (shape)	//현재 모드에 따라 그리기 실행
		{
		case SHAPE::LINE_SHAPE:	//선그리기
			MoveToEx(hdcBuff, start.x, start.y, NULL);
			LineTo(hdcBuff, end.x, end.y);
			break;
		case SHAPE::CIRCLE_SHAPE:	//원그리기
			Ellipse(hdcBuff, start.x, start.y, end.x, end.y);
			break;
		case SHAPE::RECT_SHAPE:		//네모그리기
			Rectangle(hdcBuff, start.x, start.y, end.x, end.y);
			break;
		}
		//더블 버퍼링 쓰기
		BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuff, 0, 0, SRCCOPY);
		SelectObject(hdcBuff, hBmpBuffOld);
		//delete
		DeleteObject(hBmpBuff);
		DeleteDC(hdcBuff);
		DeleteObject(pen);
		DeleteObject(brush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void SetData(int* changeData, const int* value)
{
	*changeData = *value;
}
void SetShape_Line(void) { shape = SHAPE::LINE_SHAPE; }
void SetShape_Circle(void) { shape = SHAPE::CIRCLE_SHAPE; }
void SetShape_Rect(void) { shape = SHAPE::RECT_SHAPE; }
void SetBrush_Red(void) { brushColor = COLOR::RED; }
void SetBrush_Green(void) { brushColor = COLOR::GREEN; }
void SetBrush_Blue(void) { brushColor = COLOR::BLUE; }
void SetPen_Red(void) { penColor = COLOR::RED; }
void SetPen_Green(void) { penColor = COLOR::GREEN; }
void SetPen_Blue(void) { penColor = COLOR::BLUE; }
void SetLineW_1(void) { penW = 1; }
void SetLineW_2(void){ penW = 2; }
void SetLineW_3(void){ penW = 3; }
void SetLineW_4(void){ penW = 4; }
void SetLineW_5(void){ penW = 5; }
void End(void){ PostQuitMessage(0); }