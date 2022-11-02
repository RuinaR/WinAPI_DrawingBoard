#include <windows.h>		// ������ �������
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "resource.h"

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);						//�޴� ����
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static PAINTSTRUCT ps;
	static SHAPE shape;	//������ ��� Ÿ��
	static POINT start; //�׷��� ���� ��ġ
	static POINT end;	//�׷��� �� ��ġ
	static bool isLbtnDown;	//���콺 Ŭ�� ���� �����ϴ� ����
	static bool isClear;	//ȭ�� ������ ����
	switch (iMessage)
	{
	case WM_CREATE:	//���� �ʱ�ȭ
		shape = SHAPE::NULL_SHAPE;
		start = { 0 };
		end = { 0 };
		isLbtnDown = false;
		isClear = false;
		break;
	case WM_COMMAND:
		//�޴� ���� �� ���� ��� ��带 ����
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
			InvalidateRect(hWnd, NULL, true);
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
		switch (shape)	//���� ��忡 ���� �׸��� ����
		{
		case SHAPE::LINE_SHAPE :	//���׸���
			MoveToEx(hdc, start.x, start.y, NULL);	
			LineTo(hdc, end.x, end.y);
			break;
		case SHAPE::CIRCLE_SHAPE:	//���׸���
			Ellipse(hdc, start.x, start.y, end.x, end.y);
			break;
		case SHAPE::RECT_SHAPE:		//�׸�׸���
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