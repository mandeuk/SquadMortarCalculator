// MDIpractice.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MDIpractice.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

int MapZoom;//지도의 확대 배수입니다.
int Map;//어느 지도인지 저장하는 변수입니다.
RECT clientrect;//창의 크기를 저장합니다.


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM				ChildRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ChildProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//직접제작한 함수들
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wsprintf(szTitle, L"SQ MoCal Test 0.5");
    LoadStringW(hInstance, IDC_MDIPRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	ChildRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MDIPRACTICE));

    MSG msg;

    // 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//	목적: 차일드 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditMyPos, hEditEnemPos;//에디트박스(내위치 적위치 입력용)
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
		//변수 초기화
		myposx = 0;
		myposy = 0;
		enemyposx = 0;
		enemyposy = 0;

		Map = MAP_JENSENSRANGE;
		GetClientRect(hWnd, &clientrect);
		//언어 선택
		language = ENG;

		//차일드(지도) 생성
		CreateWindow(L"Child1", NULL, WS_CHILD | WS_VISIBLE, 355, 5, clientrect.right-360, clientrect.bottom-10, hWnd, NULL, hInst, NULL);		
		//입력창 생성
		hEditMyPos = CreateWindow(L"edit", L"A123", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_UPPERCASE, 85, 50, 40, 20, hWnd, (HMENU)IDC_EDIT_MYPOS, hInst, NULL);
		hEditEnemPos = CreateWindow(L"edit", L"A123", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_UPPERCASE, 225, 50, 40, 20, hWnd, (HMENU)IDC_EDIT_ENEMPOS, hInst, NULL);		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
			case IDC_EDIT_ENEMPOS://적군 위치 입력하는 Edit창이 수정되었을 때 호출
			case IDC_EDIT_MYPOS://자신의 위치 입력하는 Edit창이 수정되었을 때 호출
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
						wsprintf(MeterText, L"%10d 미터", Distance);
						wsprintf(DegreeText, L"%10d 도", Angle);
						wsprintf(MilText, L"%10d 밀", Mil);
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

			if (language == KOR) {
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y, L"게임속 좌표가 A1-KP2-3 일 경우", 21);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 20, L"좌표입력 예시 : A123", 14);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 45, L"A : 게임속 지도상단의 알파벳", 17);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 65, L"1 : 게임속 지도좌측의 숫자", 16);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 85, L"2 : 키패드 숫자", 10);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 105, L"3 : 작은 키패드 숫자", 13);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 130, L"※. 현재 버전에서는 정확한", 15);
				TextOut(hdc, HELPTEXT_X + 20, HELPTEXT_Y + 150, L"위치계산이 불가능합니다.", 13);

				//TextOut(hdc, HELPTEXT_X + 20, HELPTEXT_Y + 150, "위치계산이 불가능합니다.", 24);
				TextOut(hdc, 10, 15, (LPCWSTR)MeterText, wcslen(MeterText));
				TextOut(hdc, 110, 15, (LPCWSTR)DegreeText, wcslen(DegreeText));
				TextOut(hdc, 210, 15, (LPCWSTR)MilText, wcslen(MilText));
				TextOut(hdc, 30, 52, L"내위치", 3);
				TextOut(hdc, 170, 52, L"적위치", 3);
				TextOut(hdc, 5, clientrect.bottom - 20, L"Copyright 2017 만득이 all rights reserved", 38);
			}
			else if (language == ENG) {
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y, L"When in-game coordinate is A1-KP2-3", 35);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 20, L"Input example : A123", 20);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 45, L"A : Ingame map Alphabat Coordinate", 34);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 65, L"1 : Ingame map Number Coordinate", 32);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 85, L"2 : Keypad number", 17);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 105, L"3 : Sub Keypad number", 21);
				TextOut(hdc, HELPTEXT_X, HELPTEXT_Y + 130, L"※. This version can calculate  ", 31);
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

// 정보 대화 상자의 메시지 처리기입니다.
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

	static CImage mapimage;//이미지 저장되는 변수
	HRESULT hResult;//이미지가 잘 불러와졌는지 결과를 저장하는 변수
	static bool bImageLoaded;//이미지가 잘 불러와졌는지 결과를 체크하는 변수

	static bool mouse_drag;//마우스 드래그 여부
	static int old_MX, old_MY, new_MX, new_MY, dist_X, dist_Y;
	static RECT childrect;
	static int mapX, mapY;//지도의 어디부터 그려줄지 좌표 저장

	static HBITMAP hBitmap, oldBitmap;//빈도화지 생성용

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

		hResult = mapimage.Load(L"resource/map/jensensrange.png");//맵 이미지 로드
		if (FAILED(hResult))//실패할 경우 bool값으로 플래그 조정
			bImageLoaded = false;
		else
			bImageLoaded = true;

		break;
	case WM_MOUSEWHEEL://마우스 휠이 입력되었을 때

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

		InvalidateRect(hWnd, NULL, FALSE);//화면갱신
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

		SetStretchBltMode(finalDC, COLORONCOLOR);//이미지 축소시 깨짐현상 방지

		//맵 그리기
		mapimage.StretchBlt(finalDC, 0, 0, childrect.right, childrect.bottom, mapX, mapY, childrect.right * MapZoom, childrect.bottom * MapZoom, SRCCOPY);
		
		//최종 드로우
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
