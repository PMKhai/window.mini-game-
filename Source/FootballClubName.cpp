// FootballClubName.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib") 
#include "FootballClubName.h"
#include <windowsx.h>
#include <time.h>
#include <vector>

//using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
int ChangeQuestion();
void NotifyResult();
bool CheckTheFC(int fcName[], int index);



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FOOTBALLCLUBNAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FOOTBALLCLUBNAME));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOOTBALLCLUBNAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FOOTBALLCLUBNAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"Football club name - Game", WS_OVERLAPPEDWINDOW,
      300, 100, 700, 450, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
class RANDOM
{
public:
	RANDOM() {
		a = rand();
	}

	~RANDOM() {}

	int GetNumber() {
		return a;
	}

	int GetNumber(int x) {
		return a % x;
	}

	void Next() {
		a = rand();
	}
private:
	int a;
};


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		//HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

HWND btnFCName1;
HWND btnFCName2;
HWND btnReplay;
HWND lblNotify; 
HWND lblQuestion;
std::vector<LPCWSTR> footballName;
std::vector<LPCWSTR> footballLogo;
HWND lblCorrectAnswer;
int Result[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int iii = 0;
int score = 0;
int numberOfQuestions = 0;
int ID = 0;
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	footballName.push_back(L"Manchester City");
	footballName.push_back(L"Paris Saint Germain");
	footballName.push_back(L"Barcelona");
	footballName.push_back(L"Bayern Munich");
	footballName.push_back(L"Lazio");
	footballName.push_back(L"Napoli");
	footballName.push_back(L"Tottenham");
	footballName.push_back(L"Inter Milan");
	footballName.push_back(L"Juventus");
	footballName.push_back(L"AS Roma");
	footballName.push_back(L"Manchester United");
	footballName.push_back(L"Valencia");
	footballName.push_back(L"Real Madrid");
	footballName.push_back(L"Chelsea");
	footballName.push_back(L"Atletico Madrid");
	footballName.push_back(L"Lyon");
	footballName.push_back(L"Monaco");
	footballName.push_back(L"Arsenal");
	footballName.push_back(L"Liverpool");
	footballName.push_back(L"Dortmund");

	footballLogo.push_back(L"ManchesterCity.png");
	footballLogo.push_back(L"ParisSaintGermain.png");
	footballLogo.push_back(L"Barcelona.png");
	footballLogo.push_back(L"BayernMunich.png");
	footballLogo.push_back(L"Lazio.png");
	footballLogo.push_back(L"Napoli.png");
	footballLogo.push_back(L"Tottenham.png");
	footballLogo.push_back(L"InterMilan.png");
	footballLogo.push_back(L"Juventus.png");
	footballLogo.push_back(L"ASRoma.png");
	footballLogo.push_back(L"ManchesterUnited.png");
	footballLogo.push_back(L"Valencia.png");
	footballLogo.push_back(L"RealMadrid.png");
	footballLogo.push_back(L"Chelsea.png");
	footballLogo.push_back(L"AtleticoMadrid.png");
	footballLogo.push_back(L"Lyon.png");
	footballLogo.push_back(L"Monaco.png");
	footballLogo.push_back(L"Arsenal.png");
	footballLogo.push_back(L"Liverpool.png");
	footballLogo.push_back(L"Dortmund.png");


	btnFCName1 = CreateWindowEx(NULL, L"BUTTON", L"FC name 1",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		150, 290, 100, 40, hwnd,
		(HMENU)IDC_FCNAME1, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnFCName1, hFont, TRUE);

	btnFCName2 = CreateWindowEx(NULL, L"BUTTON", L"FC name 2",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		300, 290, 100, 40, hwnd,
		(HMENU)IDC_FCNAME2, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnFCName2, hFont, TRUE);

	HWND lblSCORE = CreateWindowEx(NULL, L"STATIC", L"Điểm số:", 
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		500, 80, 50, 100, hwnd,
		NULL, lpCreateStruct->hInstance,NULL);
	SetWindowFont(lblSCORE, hFont, TRUE);

	lblCorrectAnswer = CreateWindowEx(NULL, L"STATIC", L"0", 
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		540, 80, 50, 50, hwnd,
		(HMENU)IDC_CORRECTANSWER, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblCorrectAnswer, hFont, TRUE);
	

	HWND lblColon = CreateWindowEx(NULL, L"STATIC", L"/", 
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		580, 80, 10, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblColon, hFont, TRUE);

	HWND lblTotalQuestion = CreateWindowEx(NULL, L"STATIC", L"10", 
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		585, 80, 50, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblTotalQuestion, hFont, TRUE);
	//HBITMAP LoadBitmap()

	HWND btnNext = CreateWindowEx(NULL, L"BUTTON", L"Bỏ qua",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		500, 100, 80, 30, hwnd,
		(HMENU)IDC_NEXTQUESTION, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnNext, hFont, TRUE);

	btnReplay = CreateWindowEx(NULL, L"BUTTON", L"Chơi",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		500, 140, 80, 30, hwnd,
		(HMENU)IDC_REPLAY, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnReplay, hFont, TRUE);

	lblNotify = CreateWindowEx(NULL, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		500, 200, 300, 300, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblNotify, hFont, TRUE);

	lblQuestion = CreateWindowEx(NULL, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		190, 20, 150, 20, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblQuestion, hFont, TRUE);

	return true;
}


void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	WCHAR buffer[3];
	switch (id)
	{
	case IDC_FCNAME1:
		
		if (numberOfQuestions < 10) {
			if (IDC_FCNAME1 == ID)
				score++;
			numberOfQuestions++;
			wsprintf(buffer, L"%d", score);
			SetWindowText(lblCorrectAnswer, buffer);

			InvalidateRect(hwnd, NULL, TRUE);
			OnPaint(hwnd);
			ID = ChangeQuestion();
		}
		if (numberOfQuestions == 10) {
			NotifyResult();
		}
		break;
	case IDC_FCNAME2:
		
		if (numberOfQuestions < 10) {
			if (IDC_FCNAME2 == ID)
				score++;
			numberOfQuestions++;
			wsprintf(buffer, L"%d", score);
			SetWindowText(lblCorrectAnswer, buffer);

			InvalidateRect(hwnd, NULL, TRUE);
			OnPaint(hwnd);
			ID = ChangeQuestion();
		}
		if (numberOfQuestions == 10) {
			NotifyResult();
		}
		break;
	case IDC_NEXTQUESTION:
		if (numberOfQuestions < 10) {
			numberOfQuestions++;
			wsprintf(buffer, L"%d", score);
			SetWindowText(lblCorrectAnswer, buffer);

			InvalidateRect(hwnd, NULL, TRUE);
			OnPaint(hwnd);
			ID = ChangeQuestion();
		}
		if (numberOfQuestions == 10) {
			NotifyResult();
		}
		break;
	case IDC_REPLAY:
		score = 0;
		numberOfQuestions = 0;
		for (iii = 0; iii < 10; iii++)
			Result[iii] = -1;
		iii = 0;

		SetWindowText(lblNotify, L"");
		SetWindowText(btnReplay, L"Chơi lại");
		SetWindowText(lblQuestion, L"Đây là đội bóng nào?");
		wsprintf(buffer, L"%d", score);
		SetWindowText(lblCorrectAnswer, buffer);

		InvalidateRect(hwnd, NULL, TRUE);
		OnPaint(hwnd);
		ID = ChangeQuestion();
		break;
	default:
		break;
	}
}
int i;
RANDOM a;
HDC hdc;
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	ULONG_PTR gdiplusToken;
	
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	srand(time(NULL));
	do {
		a.Next();
		i = a.GetNumber(20);
	} while (!CheckTheFC(Result, i));
	Result[iii] = i;
	iii++;
	{
		Graphics graphics(hdc);
		Image image(footballLogo[i]);
		graphics.DrawImage(&image, 180, 50);
	}

	GdiplusShutdown(gdiplusToken);

	EndPaint(hwnd, &ps);
}

int ChangeQuestion()
{

	a.Next();
	int temp = a.GetNumber();
	int x;
	int id = 0;
	if (temp % 2 == 0) {

		SetWindowText(btnFCName1, footballName[i]);
		a.Next();
		x = a.GetNumber(20);
		if (i == x && x != 0)
			x--;
		else if (i == x && x == 0)
			x++;
		SetWindowText(btnFCName2, footballName[x]);
		id = IDC_FCNAME1;
	}
	else {
		SetWindowText(btnFCName2, footballName[i]);
		a.Next();
		x = a.GetNumber(20);
		if (i == x && x != 0)
			x--;
		else if (i == x && x == 0)
			x++;
		SetWindowText(btnFCName1, footballName[x]);
		id = IDC_FCNAME2;
	}

	return id;
}

void NotifyResult()
{
	WCHAR buffer[260];
	wsprintf(buffer, L"Điểm số của bạn là: %d\n", score);
	SetWindowText(lblNotify, buffer);
}

bool CheckTheFC(int fcName[], int index)
{
	for (int ii = 0; ii < 10; ii++)
		if (fcName[ii] == index)
			return false;
	return true;
}