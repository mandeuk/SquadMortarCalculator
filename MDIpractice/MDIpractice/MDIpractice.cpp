// MDIpractice.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MDIpractice.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

int MapZoom;//������ Ȯ�� ����Դϴ�.
int Map;//��� �������� �����ϴ� �����Դϴ�.
RECT clientrect;//â�� ũ�⸦ �����մϴ�.


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				ChildRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ChildProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//���������� �Լ���
void TextToCor(int &myposx, int &myposy, char EditText[4]);
float CalculateAngle(int mx, int my, int ex, int ey);
int CalculateDistance(int mx, int my, int ex, int ey);
int CalculateMilliradian(int meter);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wsprintf(szTitle, L"SQ MoCal Test 0.5");
    LoadStringW(hInstance, IDC_MDIPRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	ChildRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MDIPRACTICE));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MDIPRACTICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MDIPRACTICE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//	����: ���ϵ� Ŭ������ ����մϴ�.
ATOM ChildRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ChildProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MDIPRACTICE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MDIPRACTICE);
	wcex.lpszClassName = L"Child1";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}



//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
      0, 0, 960, 700, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditMyPos, hEditEnemPos;//����Ʈ�ڽ�(����ġ ����ġ �Է¿�)
	static int myposx, myposy, enemyposx, enemyposy;
	
	static int language;

	static WCHAR MeterText[20];
	static WCHAR DegreeText[20];
	static WCHAR MilText[20];
	static char EditText[4];

    switch (message)
    {
	case WM_SIZE:
		GetClientRect(hWnd, &clientrect);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_CREATE:
		//���� �ʱ�ȭ
		myposx = 0;
		myposy = 0;
		enemyposx = 0;
		enemyposy = 0;

		Map = MAP_JENSENSRANGE;
		GetClientRect(hWnd, &clientrect);
		//��� ����
		language = ENG;

		//���ϵ�(����) ����
		CreateWindow(L"Child1", NULL, WS_CHILD | WS_VISIBLE, 355, 5, clientrect.right-360, clientrect.bottom-10, hWnd, NULL, hInst, NULL);		
		//�Է�â ����
		hEditMyPos = CreateWindow(L"edit", L"A123", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_UPPERCASE, 85, 50, 40, 20, hWnd, (HMENU)IDC_EDIT_MYPOS, hInst, NULL);
		hEditEnemPos = CreateWindow(L"edit", L"A123", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_UPPERCASE, 225, 50, 40, 20, hWnd, (HMENU)IDC_EDIT_ENEMPOS, hInst, NULL);		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
			case IDC_EDIT_ENEMPOS://���� ��ġ �Է��ϴ� Editâ�� �����Ǿ��� �� ȣ��
			case IDC_EDIT_MYPOS://�ڽ��� ��ġ �Է��ϴ� Editâ�� �����Ǿ��� �� ȣ��
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
					GetWindowText(hEditMyPos, (LPWSTR)EditText, 5);
					TextToCor(myposx, myposy, EditText);
					GetWindowText(hEditEnemPos, (LPWSTR)EditText, 5);
					TextToCor(enemyposx, enemyposy, EditText);

					int Distance = CalculateDistance(myposx, myposy, enemyposx, enemyposy);
					int Angle = (int)CalculateAngle(myposx, myposy, enemyposx, enemyposy);
					int Mil = CalculateMilliradian(Distance);

					switch (language)
					{
					case KOR:
						wsprintf(MeterText, L"%10d ����", Distance);
						wsprintf(DegreeText, L"%10d ��", Angle);
						wsprintf(MilText, L"%10d ��", Mil);
						break;
					case ENG:
						wsprintf(MeterText, L"%10d Meter", Distance);
						wsprintf(DegreeText, L"%10d Degree", Angle);
						wsprintf(MilText, L"%10d Mil", Mil);
						break;
					}

					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
				break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.

			if (language == KOR) {
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y, L"���Ӽ� ��ǥ�� A1-KP2-3 �� ���", 21);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 20, L"��ǥ�Է� ���� : A123", 14);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 45, L"A : ���Ӽ� ��������� ���ĺ�", 17);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 65, L"1 : ���Ӽ� ���������� ����", 16);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 85, L"2 : Ű�е� ����", 10);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 105, L"3 : ���� Ű�е� ����", 13);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 130, L"��. ���� ���������� ��Ȯ��", 15);
				TextOut(hdc, HELPTEXT_X + 20, HELPTEXT_Y + 150, L"��ġ����� �Ұ����մϴ�.", 13);

				//TextOut(hdc, HELPTEXT_X + 20, HELPTEXT_Y + 150, "��ġ����� �Ұ����մϴ�.", 24);
				TextOut(hdc, 10, 15, (LPCWSTR)MeterText, wcslen(MeterText));
				TextOut(hdc, 110, 15, (LPCWSTR)DegreeText, wcslen(DegreeText));
				TextOut(hdc, 210, 15, (LPCWSTR)MilText, wcslen(MilText));
				TextOut(hdc, 30, 52, L"����ġ", 3);
				TextOut(hdc, 170, 52, L"����ġ", 3);
				TextOut(hdc, 5, clientrect.bottom - 20, L"Copyright 2017 ������ all rights reserved", 38);
			}
			else if (language == ENG) {
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y, L"When in-game coordinate is A1-KP2-3", 35);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 20, L"Input example : A123", 20);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 45, L"A : Ingame map Alphabat Coordinate", 34);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 65, L"1 : Ingame map Number Coordinate", 32);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 85, L"2 : Keypad number", 17);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 105, L"3 : Sub Keypad number", 21);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 130, L"��. This version can calculate  ", 31);
				TextOut(hdc, HELPTEXT_X + 20, HELPTEXT_Y + 150, L"Approximate Value", 17);


				TextOut(hdc, 30, 52, L"MyPos", 5);
				TextOut(hdc, 150, 52, L"EnemyPos", 8);
				TextOut(hdc, 5, clientrect.bottom - 20, L"Copyright 2017 Mandeuk all rights reserved", 42);
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC, finalDC;

	static CImage mapimage;//�̹��� ����Ǵ� ����
	HRESULT hResult;//�̹����� �� �ҷ��������� ����� �����ϴ� ����
	static bool bImageLoaded;//�̹����� �� �ҷ��������� ����� üũ�ϴ� ����

	static bool mouse_drag;//���콺 �巡�� ����
	static int old_MX, old_MY, new_MX, new_MY, dist_X, dist_Y;
	static RECT childrect;
	static int mapX, mapY;//������ ������ �׷����� ��ǥ ����

	static HBITMAP hBitmap, oldBitmap;//��ȭ�� ������

	switch (uMsg) {
	case WM_SIZE:
		//GetClientRect(hWnd, &clientrect);
		//InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CREATE:
		MapZoom = MAP_MINZOOM;

		mouse_drag = false;

		GetClientRect(hWnd, &childrect);
		mapX = 0;
		mapY = 0;

		hResult = mapimage.Load(L"resource/map/jensensrange.png");//�� �̹��� �ε�
		if (FAILED(hResult))//������ ��� bool������ �÷��� ����
			bImageLoaded = false;
		else
			bImageLoaded = true;

		break;
	case WM_MOUSEWHEEL://���콺 ���� �ԷµǾ��� ��

		switch (GET_WHEEL_DELTA_WPARAM(wParam))
		{
		case MOUSE_WHEELUP:
			if (MapZoom > MAP_MINZOOM)
				MapZoom--;
			break;
		case MOUSE_WHEELDOWN:
			if (MapZoom < MAP_MAXZOOM)
				MapZoom++;
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);//ȭ�鰻��
		break;

	case WM_LBUTTONDOWN:
		if (mouse_drag == false) {
			mouse_drag = true;
			old_MX = LOWORD(lParam);
			old_MY = HIWORD(lParam);
			new_MX = old_MX;
			new_MY = old_MY;
		}
		break;
	case WM_LBUTTONUP:
		if(mouse_drag == true)
			mouse_drag = false;
		break;
	case WM_MOUSEMOVE:
		if (mouse_drag == true)
		{
			new_MX = LOWORD(lParam);
			new_MY = HIWORD(lParam);
			dist_X = new_MX - old_MX;
			dist_Y = new_MY - old_MY;
			mapX -= dist_X * MapZoom;
			mapY -= dist_Y * MapZoom;
			if (mapX < 0)
				mapX = 0;
			if (mapY < 0)
				mapY = 0;
			old_MX = new_MX;
			old_MY = new_MY;
			InvalidateRect(hWnd, NULL, FALSE);
		}

		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		finalDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, childrect.right, childrect.bottom);
		oldBitmap = (HBITMAP)SelectObject(finalDC, hBitmap);

		SetStretchBltMode(finalDC, COLORONCOLOR);//�̹��� ��ҽ� �������� ����

		//�� �׸���
		mapimage.StretchBlt(finalDC, 0, 0, childrect.right, childrect.bottom, mapX, mapY, childrect.right * MapZoom, childrect.bottom * MapZoom, SRCCOPY);
		
		//���� ��ο�
		BitBlt(hDC, 0, 0, childrect.right, childrect.bottom, finalDC, 0, 0, SRCCOPY);

		
		
		SelectObject(finalDC, oldBitmap);
		DeleteObject(hBitmap);
		DeleteObject(oldBitmap);
		DeleteDC(finalDC);
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return 0;
}


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
	if (cal == 0)
		return MilMeter[cal];
	else
		return MilMeter[cal - 1] - ((meter % 50) * (MilGap[cal] / 100.0f));

}
