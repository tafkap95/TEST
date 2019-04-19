#include <windows.h>
#include <stdio.h>
#include <iostream>
#define _WIN32_IE 0x301
#include <commctrl.h>
#include "resource.h"
#include "main.h"
#include "functions.h"

using namespace std;

#include "common.h"

const char g_szClassName[] = "myWindowClass 2.0";

vector<vector<string> > dir_by_cat
{
    { "FILM", "C:\\TEST_FILMS\\DESTINATION\\FILMS" },
    { "MUSIQUE", "C:\\TEST_FILMS\\DESTINATION\\MUSIQUE" },
};

string g_path = "C:\\TEST_FILMS";
HINSTANCE hInst;

void CALLBACK MyTimerProc(
    HWND hwnd,        // handle to window for timer messages
    UINT message,     // WM_TIMER message
    UINT idTimer,     // timer identifier
    DWORD dwTime)     // current system time
{
    exec_process();
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
    case WM_INITDIALOG:

        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hwnd, IDOK);
            break;
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

void GamePaint(HDC hDC)
{
    MoveToEx(hDC, 0, 0, NULL);
    LineTo(hDC, 50, 50);

    TextOut(hDC, 50, 100, TEXT("Michael Morrison"), 16);
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {

//        case WM_INITDIALOG:
//			{
//			    int i;
//
//			    i = 0;
//
//			    break;
//			}

    case WM_CREATE:
    {
        AddControls(hwnd);
        //  CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("0"), WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|WS_TABSTOP, 20, 10, 85, 25, hwnd, (HMENU)IDC_LHS, GetModuleHandle(NULL), NULL);

        break;
    }

    case WM_PAINT:
    {
        HDC     hDC;
        PAINTSTRUCT ps;
        hDC = BeginPaint(hwnd, &ps);

// Paint the game
        GamePaint(hDC);

        EndPaint(hwnd, &ps);



        break;


//        PAINTSTRUCT ps;
//        HDC hdc = BeginPaint (hwnd, &ps);
//        TextOut (hdc,
//                 // Location of the text
//                 10,
//                 10,
//                 // Text to print
//                 TextArray [0],
//                 // Size of the text, my function gets this for us
//                 GetTextSize (TextArray [0]));
//        EndPaint (hwnd, &ps);
//        PAINTSTRUCT ps;
//        RECT rec;
//
//        HDC hdc = BeginPaint(hwnd, &ps);
//
//        SetTextColor(hdc, RGB(255,255,255));
//        SetRect(&rec,10,10,100,100);
//
//       // FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW));
//
//        DrawText(hdc, TEXT("Text Out String"),strlen("Text Out String"),&rec, DT_TOP|DT_LEFT);
//
//        EndPaint(hwnd, &ps);
//
//         ReleaseDC(hwnd, hdc);



    }


    case WM_COMMAND:

        switch(LOWORD(wParam))
        {
        case ID_BTN_START:
        {
            printf("Timer Start");
            SetTimer(hwnd,                      // handle to main window
                     IDT_TIMER1,                // timer identifier
                     2000,                      // 5-second interval
                     (TIMERPROC) MyTimerProc);   // timer callback
            break;
        }

        case ID_BTN_STOP:
        {
            KillTimer(hwnd, IDT_TIMER1);
            printf("Timer Stop");
            break;
        }

        case ID_BTN_EXIT:
        {
            KillTimer(hwnd, IDT_TIMER1);
            PostQuitMessage(0);
            break;
        }

        case ID_FILE_EXIT:
        {
            KillTimer(hwnd, IDT_TIMER1);
            PostQuitMessage(0);
            break;
        }

        case ID_OPTIONS:
        {
            //    int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DIAL_PREF), hwnd, AboutDlgProc);
            break;
        }

        case ID_HELP_ABOUT:
        {
            printf("ID_HELP_ABOUT");
            break;
        }

        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    init_process();

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    HINSTANCE hInst;

    hInst = hInstance;

    int horizontal = 0;
    int vertical = 0;
    GetDesktopResolution(horizontal, vertical);

    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
    InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    BOOL bRet = InitCommonControlsEx(&InitCtrls);

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MYMENU);
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_QUESTION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
               WS_EX_CLIENTEDGE,
               g_szClassName,
               g_szClassName,
               WS_OVERLAPPED,
               CW_USEDEFAULT, CW_USEDEFAULT, (horizontal / 2 ), ( vertical / 2 ),
               NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    else
    {
        CreatemyToolbar(hwnd, hInst);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
