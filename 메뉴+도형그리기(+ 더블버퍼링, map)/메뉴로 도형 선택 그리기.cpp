#include <windows.h>		// 윈도우 헤더파일
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "resource.h"

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);						//메뉴 지정
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static PAINTSTRUCT ps;
	static SHAPE shape;	//선택한 모양 타입
	static POINT start; //그려질 시작 위치
	static POINT end;	//그려질 끝 위치
	static bool isLbtnDown;	//마우스 클릭 상태 저장하는 변수
	static bool isClear;	//화면 지우기용 변수
	switch (iMessage)
	{
	case WM_CREATE:	//변수 초기화
		shape = SHAPE::NULL_SHAPE;
		start = { 0 };
		end = { 0 };
		isLbtnDown = false;
		isClear = false;
		break;
	case WM_COMMAND:
		//메뉴 선택 시 현재 모양 모드를 변경
		switch (LOWORD(wParam))
		{
		case ID_LINE:
			shape = SHAPE::LINE_SHAPE;
			break;
		case ID_CIRCLE:
			shape = SHAPE::CIRCLE_SHAPE;
			break;
		case ID_RECT:
			shape = SHAPE::RECT_SHAPE;
			break;
		case ID_END:
			DestroyWindow(hWnd);
			break;
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
			InvalidateRect(hWnd, NULL, true);
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
		switch (shape)	//현재 모드에 따라 그리기 실행
		{
		case SHAPE::LINE_SHAPE :	//선그리기
			MoveToEx(hdc, start.x, start.y, NULL);	
			LineTo(hdc, end.x, end.y);
			break;
		case SHAPE::CIRCLE_SHAPE:	//원그리기
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			break;
		case SHAPE::RECT_SHAPE:		//네모그리기
			Rectangle(hdc, start.x, start.y, end.x, end.y);
			break;
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}