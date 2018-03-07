#pragma once
#include "main.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "SQ MoCal v0.2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void TextToCor(int &myposx, int &myposy, char EditText[4]);
float CalculateAngle(int mx, int my, int ex, int ey);
int CalculateDistance(int mx, int my, int ex, int ey);
int CalculateMilliradian(int meter);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	//srand((unsigned)time(NULL));////////

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;//(WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;

	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	WndClass.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, W_WIDTH + 16, W_HEIGHT + 38, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}//end of WinMain



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC, finalDC;

	//static CImage help;
	//HRESULT hResult;
	//static bool bImageLoaded;

	static HWND hButton, hEdit;
	static HWND hEditMyPos, hEditEnemPos;//에디트박스(내위치 적위치 입력용)
	static char EditText[4];
	static char ResultText[32];

	static int myposx, myposy, enemyposx, enemyposy;

	static bool Language;

	switch (iMessage)
	{
	case WM_CREATE:
		myposx = 0;
		myposy = 0;
		enemyposx = 0;
		enemyposy = 0;
		Language = true;

		//hResult = help.Load("resource/HELP.png");
		//if (FAILED(hResult))
		//	bImageLoaded = false;
		//else
		//	bImageLoaded = true;

		//hButton = CreateWindow("button", "확인"/*버튼에 적힌 글씨*/, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 0, 100, 25, hWnd, (HMENU)IDC_BUTTON, g_hInst, NULL);
		hEdit = CreateWindow("edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 30, 20, 235, 25, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);

		hEditMyPos = CreateWindow("edit", "A123", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_UPPERCASE, 85, 50, 40, 20, hWnd, (HMENU)IDC_EDIT_MYPOS, g_hInst, NULL);
		hEditEnemPos = CreateWindow("edit", "A123", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_UPPERCASE, 225, 50, 40, 20, hWnd, (HMENU)IDC_EDIT_ENEMPOS, g_hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_ENEMPOS:
		case IDC_EDIT_MYPOS:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				GetWindowText(hEditMyPos, EditText, 5);
				TextToCor(myposx, myposy, EditText);
				GetWindowText(hEditEnemPos, EditText, 5);
				TextToCor(enemyposx, enemyposy, EditText);

				int Distance = CalculateDistance(myposx, myposy, enemyposx, enemyposy);
				int Angle = (int)CalculateAngle(myposx, myposy, enemyposx, enemyposy);
				int Mil = CalculateMilliradian(Distance);
				if(Language)
					sprintf(ResultText, "%6d 미터, %6d 도, %6d 밀", Distance, Angle, Mil);
				else
					sprintf(ResultText, "%5d Meter, %5d Degree, %5d Mil", Distance, Angle, Mil);

				SetWindowText(hEdit, ResultText);
				break;
			}
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case VK_RETURN://엔터
			Language = !Language;
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		if (HIWORD(lParam) <300)
			SetFocus(hWnd);
		else
			ShellExecute(hWnd, "open", "http://playgrammer.tistory.com/category/Game/SQUAD", NULL, NULL, SW_SHOW);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		if (Language == true) {
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y, "게임속 좌표가 A1-KP2-3 일 경우", 30);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 20, "좌표입력 예시 : A123", 20);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 45, "A : 게임속 지도상단의 알파벳", 28);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 65, "1 : 게임속 지도좌측의 숫자", 26);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 85, "2 : 키패드 숫자", 15);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 105, "3 : 작은 키패드 숫자", 20);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 130, "※. 현재 버전에서는 정확한", 26);
			TextOut(hDC, HELPTEXT_X + 20, HELPTEXT_Y + 150, "위치계산이 불가능합니다.", 24);

			//TextOut(hDC, HELPTEXT_X + 20, HELPTEXT_Y + 150, "위치계산이 불가능합니다.", 24);
			TextOut(hDC, 30, 52, "내위치", 6);
			TextOut(hDC, 170, 52, "적위치", 6);
			TextOut(hDC, 220, HELPTEXT_Y + 185, "Enter : ENG", 11);
			TextOut(hDC, 5, HELPTEXT_Y + 200, "Copyright 2017 만득이 all rights reserved", 41);
		}
		else {
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y, "When in-game coordinate is A1-KP2-3", 35);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 20, "Input example : A123", 20);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 45, "A : Ingame map Alphabat Coordinate", 34);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 65, "1 : Ingame map Number Coordinate", 32);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 85, "2 : Keypad number", 17);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 105, "3 : Sub Keypad number", 21);
			TextOut(hDC, HELPTEXT_X, HELPTEXT_Y + 130, "※. This version can calculate  ", 32);
			TextOut(hDC, HELPTEXT_X + 20, HELPTEXT_Y + 150, "Approximate Value", 17);


			TextOut(hDC, 30, 52, "MyPos", 5);
			TextOut(hDC, 150, 52, "EnemyPos", 8);
			TextOut(hDC, 220, HELPTEXT_Y + 185, "Enter : KOR", 11);
			TextOut(hDC, 5, HELPTEXT_Y + 200, "Copyright 2017 Mandeuk all rights reserved", 42);
		}
		TextOut(hDC, 5, HELPTEXT_Y + 185, VERSION, 4);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}//end of switch(iMessage)
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}//end of CALLBACK WndProc

void TextToCor(int &myposx, int &myposy, char EditText[4])
{
	myposx = ((int)(EditText[0] - 65) * GRID_DISTANCE);
	myposy = ((int)(EditText[1] - 49) * GRID_DISTANCE);

	int keypadnum = EditText[2] - 48;
	switch (keypadnum) {
	case 1: myposy += KP_DISTANCE * 2;
		break;
	case 2: myposx += KP_DISTANCE * 1; myposy += KP_DISTANCE * 2;
		break;
	case 3: myposx += KP_DISTANCE * 2; myposy += KP_DISTANCE * 2;
		break;
	case 4: myposy += KP_DISTANCE * 1;
		break;
	case 5: myposx += KP_DISTANCE * 1; myposy += KP_DISTANCE * 1;
		break;
	case 6: myposx += KP_DISTANCE * 2; myposy += KP_DISTANCE * 1;
		break;
	case 7:
		break;
	case 8: myposx += KP_DISTANCE * 1;
		break;
	case 9: myposx += KP_DISTANCE * 2;
		break;
	}

	int smallkeypadnum = EditText[3] - 48;
	switch (smallkeypadnum) {
	case 1: myposy += SKP_DISTANCE * 2;
		break;
	case 2: myposx += SKP_DISTANCE * 1; myposy += SKP_DISTANCE * 2;
		break;
	case 3: myposx += SKP_DISTANCE * 2; myposy += SKP_DISTANCE * 2;
		break;
	case 4: myposy += SKP_DISTANCE * 1;
		break;
	case 5: myposx += SKP_DISTANCE * 1; myposy += SKP_DISTANCE * 1;
		break;
	case 6: myposx += SKP_DISTANCE * 2; myposy += SKP_DISTANCE * 1;
		break;
	case 7:
		break;
	case 8: myposx += SKP_DISTANCE * 1;
		break;
	case 9: myposx += SKP_DISTANCE * 2;
		break;
	}
}

float CalculateAngle(int mx, int my, int ex, int ey)
{
	float fAngle;
	float cx, cy;

	cx = (float)mx - (float)ex;
	cy = (float)my - (float)ey;
	if (cy == 0.0f)
		cy = 0.000001f;
	fAngle = atan(-cx / cy);
	if (cy < 0) fAngle += PI;
	if (ex <= mx && ey <= my) fAngle += 2 * PI;

	return (fAngle * 57.295791f);
}

int CalculateDistance(int mx, int my, int ex, int ey)
{
	return sqrt(pow(ex - mx, 2) + pow(ey - my, 2));
}

int CalculateMilliradian(int meter)//added in v0.2
{
	int cal;
	if (meter <= 50)
		cal = 0;
	else if (meter >= 1250)
		cal = 25;
	else
		cal = meter / 50;
	if(cal == 0)
		return MilMeter[cal];
	else
		return MilMeter[cal-1] - ((meter % 50) * (MilGap[cal] / 100.0f));
	
}