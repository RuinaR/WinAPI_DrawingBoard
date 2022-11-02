#include <windows.h>		// ������ �������
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <map>
#include "resource.h"
using namespace std;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;										//�ν��Ͻ� �ڵ�
LPCTSTR lpszClass = TEXT("�ֿ���_22311019");				//���� ǥ���ٿ� ǥ�õǴ� ����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam,
	int nCmdShow)
{
	HWND hWnd;											//������ �ڵ� ����
	MSG Message;										//�޼��� ����ü ���� ����
	WNDCLASS WndClass;									//Windows Class ����ü ���� ����
	g_hInst = hInstance;								//hInstance���� �ܺο����� ����� �� �ֵ��� ���������� ���� ����

	WndClass.cbClsExtra = 0;							//���� ����. ������ ���X
	WndClass.cbWndExtra = 0;							//���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// �������� ��� ������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//�������� ���콺������ ����� ����
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//�������� Ŀ�� ������ ��� ����
	WndClass.hInstance = hInstance;						//������ Ŭ������ ����ϴ� ���α׷� ��ȣ
	WndClass.lpfnWndProc = WndProc;						//������ �޼��� ó�� �Լ� ����
	WndClass.lpszClassName = lpszClass;					//������ Ŭ������ �̸� ����
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU3);						//�޴� ����
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//�������� ��Ÿ���� ����

	RegisterClass(&WndClass);							//WNDCLASS ����ü�� ������ ����


	hWnd = CreateWindow(lpszClass, lpszClass,			//�����츦 ����
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		/* 100,100,500,500*/, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	//�޼��� ������ ����
	while (GetMessage(&Message, NULL, 0, 0)) {			//Queue�� �ִ� �޼����� �о���δ�
		TranslateMessage(&Message);						//Ű���� �Է� �޼����� �����Ͽ� ���α׷����� ���� 
														//  ����� �� �ֵ���
		DispatchMessage(&Message);						//�޼����� �������� �޼��� ó�� �Լ� WndProc�� ����
	}
	return (int)Message.wParam;							//Ż�� �ڵ�. ���α׷� ����
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

SHAPE shape;	//������ ��� Ÿ��
COLOR brushColor;	//�귯�� ��
COLOR penColor;	//�� ��
int penW;	//�� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static PAINTSTRUCT ps;

	//�׸��⿡ ����� ����
	static POINT start; //�׷��� ���� ��ġ
	static POINT end;	//�׷��� �� ��ġ
	static bool isLbtnDown;	//���콺 Ŭ�� ���� �����ϴ� ����
	static bool isClear;	//ȭ�� ������ ����
	//�Լ������� map
	static map<int, void(*)(void)> funcMap;
	static map<int, void(*)(void)>::iterator iter;
	//������۸��� ����
	static HDC hdcBuff;
	HBITMAP hBmpBuff;
	HBITMAP hBmpBuffOld;
	RECT clientRect;
	//brush, pen
	static HBRUSH brush;
	static HPEN pen;
	switch (iMessage)
	{
	case WM_CREATE:	//���� �ʱ�ȭ, �⺻����
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
	case WM_LBUTTONDOWN:	//��Ŭ�� �� 
		//�׸� ���� ��ǥ ����
		isLbtnDown = true;
		start.x = LOWORD(lParam);
		start.y = HIWORD(lParam);
		//ȭ�� Ŭ����
		isClear = true;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MOUSEMOVE:
		//��Ŭ�� ���·� �����̸� �׷��� �� ��ǥ�� �����ϰ� WM_PAINT ����
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
		if (isClear)	//ȭ�� Ŭ������ ���
		{
			isClear = false;	//false�� �ٲٰ�
			break;				//�ƹ��͵� ���ϰ� ����
		}

		hdc = BeginPaint(hWnd, &ps);

		//���� ���۸��� DC, BMP
		GetClientRect(hWnd, &clientRect);
		hdcBuff = CreateCompatibleDC(hdc);
		hBmpBuff = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
		hBmpBuffOld = (HBITMAP)SelectObject(hdcBuff, hBmpBuff);
		//PatBlt(hdcBuff, 0, 0, clientRect.right, clientRect.bottom, WHITENESS);
		FillRect(hdcBuff, &clientRect, (HBRUSH)COLOR_WINDOWFRAME);
		//�귯��, �� ����
		brush = CreateSolidBrush(arrColor[brushColor]);
		pen = CreatePen(PS_SOLID, penW, arrColor[penColor]);
		//�귯��, �� ���
		SelectObject(hdcBuff, brush);
		SelectObject(hdcBuff, pen);
		switch (shape)	//���� ��忡 ���� �׸��� ����
		{
		case SHAPE::LINE_SHAPE:	//���׸���
			MoveToEx(hdcBuff, start.x, start.y, NULL);
			LineTo(hdcBuff, end.x, end.y);
			break;
		case SHAPE::CIRCLE_SHAPE:	//���׸���
			Ellipse(hdcBuff, start.x, start.y, end.x, end.y);
			break;
		case SHAPE::RECT_SHAPE:		//�׸�׸���
			Rectangle(hdcBuff, start.x, start.y, end.x, end.y);
			break;
		}
		//���� ���۸� ����
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