#include <windows.h>
#include <tchar.h>
#include <cmath>
#include <string>

/*   CONFIG DATA   */
bool GetDataFromXvirtualMachine__ = true;
bool V86Mode__ = true;
bool LoadOSInstenseFromXvirtualMachine__ = true;
bool AllStartupInterfaceLoaded__ = true;
bool SafeLoad__ = true;
/*   CONFIG DATA   */
bool LoadStartupOS__;

bool isFullscreen = false;
bool isCursorHidden = false;
WINDOWPLACEMENT prevWindowPlacement = { sizeof(WINDOWPLACEMENT) };
DWORD prevWindowStyle;
RECT prevWindowRect;

//Xvirtual
HWND __STARTUP_MSG__;
HWND __ENTERED_MSG__;
HWND __STARTUP_OK__;
HWND __STARTUP_OK2__;
HWND __GetExcuteFiles_xVirtual__;
HWND __LOGO_Xvirtual__;
HWND __VLOGO_Xvirtual__;
HWND __SLOGO_Xvirtual__;
HWND __SELECTLINE1_OS__;
HWND __SELECTLINE2_OS__;
HWND __SELECTLINE3_OS__;
HWND __SELECTLINE4_OS__;
HWND __SELECTLINE5_OS__;
HWND __SELECTOS1_OS__;
HWND __SELECTOS2_OS__;
HWND __SELECTOS3_OS__;
HWND __SELECTCS1_OS__;
HWND __SELECTCS2_OS__;
HWND __SELECTCS3_OS__;
HWND __EMPTY_DISPLAY__;
HWND __EMPTY2_DISPLAY__;
HWND __EMPTY3_DISPLAY__;
HWND __EMPTY_DISPLAY_LOGO__;
//Xvirtual

HFONT __STARTUP_FONT__;
HFONT __LOGO_FONT_Xvirtual__;
HFONT __SELECT_FONT_Xvirtual__;

HBRUSH hbrBkgnd = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int selectedOSIndex = 0;
HWND selectOSWindows[3];

int APIENTRY wWinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    static wchar_t szAppName[] = L"Xvirtual OS Machine";
    HWND           hWnd;
    MSG            msg;
    WNDCLASS       wndclass = {};

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        return EXIT_FAILURE;
    }

    hWnd = CreateWindowW(
        szAppName,
        L"MyOS",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void ToggleFullscreen(HWND hwnd) {
    if (isFullscreen) {
        // Restore the window to its previous style and position
        SetWindowLong(hwnd, GWL_STYLE, prevWindowStyle);
        SetWindowPlacement(hwnd, &prevWindowPlacement);
        SetWindowPos(hwnd, NULL, prevWindowRect.left, prevWindowRect.top, prevWindowRect.right - prevWindowRect.left, prevWindowRect.bottom - prevWindowRect.top,
            SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        isFullscreen = false;
    }
    else {
        // Save the current window style and position
        prevWindowStyle = GetWindowLong(hwnd, GWL_STYLE);
        GetWindowPlacement(hwnd, &prevWindowPlacement);
        GetWindowRect(hwnd, &prevWindowRect);

        // Set the window style to borderless and maximize it to full screen
        SetWindowLong(hwnd, GWL_STYLE, prevWindowStyle & ~WS_OVERLAPPEDWINDOW);
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        SetWindowPos(hwnd, NULL, 0, 0, screenWidth, screenHeight, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        isFullscreen = true;
    }
}

void ShowCursor(bool show) {
    if (show) {
        while (ShowCursor(TRUE) < 0);
        isCursorHidden = false;
    }
    else {
        while (ShowCursor(FALSE) >= 0);
        isCursorHidden = true;
    }
}

void UpdateSelectionDisplay() {
    for (int i = 0; i < 3; ++i) {
        if (i == selectedOSIndex) {
            SetBkColor(GetDC(selectOSWindows[i]), RGB(100, 100, 100));
            SetWindowText(selectOSWindows[i], L"       NAME                    For Xvirtual OS Machine               ");
        }
        else {
            SetBkColor(GetDC(selectOSWindows[i]), RGB(50, 50, 50));
            SetWindowText(selectOSWindows[i], L"       NAME                    For Xvirtual OS Machine               ");
        }
    }
}

void ClearScreen(HWND hWnd) {
    HWND hChild = GetWindow(hWnd, GW_CHILD);
    while (hChild) {
        HWND hNextChild = GetWindow(hChild, GW_HWNDNEXT);
        DestroyWindow(hChild);
        hChild = hNextChild;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hDC;
    PAINTSTRUCT ps;
    RECT        rect;
    LOGFONT lf = {};
    HWND hwndStatic = (HWND)lParam;

    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            ToggleFullscreen(hWnd);
        }
        else if (wParam == VK_DOWN) {
            selectedOSIndex = (selectedOSIndex + 1) % 3;
            UpdateSelectionDisplay();
        }
        else if (wParam == VK_UP) {
            selectedOSIndex = (selectedOSIndex + 2) % 3;
            UpdateSelectionDisplay();
        }
        else if (wParam == 'E') {
            //function
        }
        else if (wParam == 'Q') {
            Sleep(2000);
            DestroyWindow(hWnd);
        }
        break; // Add break to avoid fall-through

    case WM_MOUSEMOVE:
        if (!isCursorHidden) {
            ShowCursor(false);
        }
        break;

    case WM_CREATE:
        hbrBkgnd = CreateSolidBrush(RGB(0, 0, 0)); // Initialize the background brush

        __STARTUP_MSG__ = CreateWindowEx(
            0,
            L"STATIC",
            L"Welcome to Xvirtual OS Machine.",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            10,
            10,
            500,
            25,
            hWnd,
            (HMENU)0,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        __ENTERED_MSG__ = CreateWindowEx(
            0,
            L"STATIC",
            L"Get Data from Xvirtual OS Machine...",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            10,
            40,
            500,
            25,
            hWnd,
            (HMENU)1,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        __GetExcuteFiles_xVirtual__ = CreateWindowEx(
            0,
            L"STATIC",
            L"Excute Xvirtual files and change to color mode...",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            10,
            60,
            500,
            25,
            hWnd,
            (HMENU)2,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        if (!GetDataFromXvirtualMachine__) {
            MessageBox(hWnd, L"Failed to get data from Xvirtual OS Machine.", L"Error", MB_ICONERROR);
            return -1;
        }
        else {
            __STARTUP_OK__ = CreateWindowEx(
                0,
                L"STATIC",
                L"[ OK ]",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                300,
                40,
                500,
                25,
                hWnd,
                (HMENU)10,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL
            );
        }

        if (!V86Mode__) {
            MessageBox(hWnd, L"change to color mode (V86 Mode) failed.", L"Error", MB_ICONERROR);
            return -1;
        }
        else {
            __STARTUP_OK2__ = CreateWindowEx(
                0,
                L"STATIC",
                L"[ OK ]",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                300,
                60,
                500,
                25,
                hWnd,
                (HMENU)10,
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL
            );
        }

        __LOGO_Xvirtual__ = CreateWindowEx(
            0,
            L"STATIC",
            L"Xvirtual",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            900,
            625,
            500,
            100,
            hWnd,
            (HMENU)7,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        __VLOGO_Xvirtual__ = CreateWindowEx(
            0,
            L"STATIC",
            L"Xvirtual",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            900,
            675,
            500,
            40,
            hWnd,
            (HMENU)8,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        __SELECTLINE1_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"┌────────────┐",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            685,
            300,
            1000,
            50,
            hWnd,
            (HMENU)9,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        __SELECTLINE2_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"│                              │",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            691,
            350,
            1000,
            50,
            hWnd,
            (HMENU)9,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        __SELECTLINE3_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"│                              │",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            691,
            400,
            1000,
            50,
            hWnd,
            (HMENU)9,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        __SELECTLINE4_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"│                              │",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            691,
            450,
            1000,
            50,
            hWnd,
            (HMENU)9,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        __SELECTLINE5_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"└────────────┘",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            686,
            500,
            1000,
            50,
            hWnd,
            (HMENU)9,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        __SELECTOS1_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"    EMPTY                      For Xvirtual OS Machine                      ",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            750,
            380,
            1000,
            50,
            hWnd,
            (HMENU)11,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        __SELECTOS2_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"    EMPTY                      For Xvirtual OS Machine                      ",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            750,
            430,
            1000,
            50,
            hWnd,
            (HMENU)11,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        __SELECTOS3_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"      EMPTY                      For Xvirtual OS Machine                      ",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            750,
            480,
            1000,
            50,
            hWnd,
            (HMENU)11,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        selectOSWindows[0] = __SELECTOS1_OS__;
        selectOSWindows[1] = __SELECTOS2_OS__;
        selectOSWindows[2] = __SELECTOS3_OS__;
        UpdateSelectionDisplay();

        if (!LoadOSInstenseFromXvirtualMachine__) {
            MessageBox(hWnd, L"Failed to load OS Instance from Xvirtual OS Machine.", L"Error", MB_ICONERROR);
            return -1;
        }

        __SELECTCS1_OS__ = CreateWindowEx(
            0,
            L"STATIC",
            L"Press \"Arrow key\" to select OS. Press\"E\" Key to get in to booting mode.",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            800,
            750,
            1000,
            50,
            hWnd,
            (HMENU)12,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        hwndStatic = (HWND)lParam;

        lf.lfHeight = -MulDiv(10, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
        lf.lfWeight = FW_NORMAL;
        __STARTUP_FONT__ = CreateFontIndirect(&lf);

        lf.lfHeight = -MulDiv(50, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
        lf.lfWeight = FW_NORMAL;
        __LOGO_FONT_Xvirtual__ = CreateFontIndirect(&lf);

        lf.lfHeight = -MulDiv(15, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
        lf.lfWeight = FW_NORMAL;
        __SELECT_FONT_Xvirtual__ = CreateFontIndirect(&lf);

        SendMessage(__STARTUP_MSG__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);
        SendMessage(__ENTERED_MSG__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);
        SendMessage(__STARTUP_OK__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);
        SendMessage(__STARTUP_OK2__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);
        SendMessage(__GetExcuteFiles_xVirtual__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);
        SendMessage(__GetExcuteFiles_xVirtual__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);
        SendMessage(__SELECTCS1_OS__, WM_SETFONT, (WPARAM)__STARTUP_FONT__, TRUE);

        SendMessage(__LOGO_Xvirtual__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);
        SendMessage(__VLOGO_Xvirtual__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTLINE1_OS__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTLINE2_OS__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTLINE3_OS__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTLINE4_OS__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTLINE5_OS__, WM_SETFONT, (WPARAM)__LOGO_FONT_Xvirtual__, TRUE);

        SendMessage(__SELECTOS1_OS__, WM_SETFONT, (WPARAM)__SELECT_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTOS3_OS__, WM_SETFONT, (WPARAM)__SELECT_FONT_Xvirtual__, TRUE);
        SendMessage(__SELECTOS2_OS__, WM_SETFONT, (WPARAM)__SELECT_FONT_Xvirtual__, TRUE);

        if (!AllStartupInterfaceLoaded__) {
            MessageBox(hWnd, L"Instance configuration failed.", L"Error", MB_ICONERROR);
            return -1;
        }

        if (!SafeLoad__) {
            MessageBox(hWnd, L"Failed to safe loading", L"Error", MB_ICONERROR);
            LoadStartupOS__ = false;
        }
        else {
            LoadStartupOS__ = true;
        }

        ToggleFullscreen(hWnd);

        return 0;

    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rect);
        // Perform painting here, if needed
        EndPaint(hWnd, &ps); // Ensure to call EndPaint
        return 0;

    case WM_CTLCOLORSTATIC:
        hDC = (HDC)wParam;
        hwndStatic = (HWND)lParam;

        if (hwndStatic == __STARTUP_OK2__) {
            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, RGB(0, 255, 0));
        }
        else if (hwndStatic == __LOGO_Xvirtual__) {
            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, RGB(180, 180, 180));
        }
        else if (hwndStatic == __VLOGO_Xvirtual__ || hwndStatic == __SELECTLINE1_OS__ || hwndStatic == __SELECTLINE2_OS__ || hwndStatic == __SELECTLINE3_OS__ || hwndStatic == __SELECTLINE4_OS__ || hwndStatic == __SELECTLINE5_OS__) {
            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, RGB(80, 80, 80));
        }
        else if (hwndStatic == __SELECTOS1_OS__ || hwndStatic == __SELECTOS2_OS__ || hwndStatic == __SELECTOS3_OS__) {
            if (hwndStatic == selectOSWindows[selectedOSIndex]) {
                SetBkColor(hDC, RGB(100, 100, 100));
            }
            else {
                SetBkColor(hDC, RGB(50, 50, 50));
            }
            SetTextColor(hDC, RGB(200, 200, 200));
        }
        else {
            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, RGB(255, 255, 255));
        }

        return (INT_PTR)hbrBkgnd; // Return the background brush for all static controls

    case WM_DESTROY:
        DeleteObject(__STARTUP_FONT__);
        DeleteObject(__LOGO_FONT_Xvirtual__);
        DeleteObject(__SELECT_FONT_Xvirtual__);
        DeleteObject(hbrBkgnd); // Clean up the background brush
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        if (isFullscreen) {
            // Ensure the window stays in fullscreen mode when resized
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            SetWindowPos(hWnd, NULL, 0, 0, screenWidth, screenHeight, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
