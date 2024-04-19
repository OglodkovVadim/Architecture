#include <windows.h>
#include <iostream>
#include <fstream>
#include "resource.h"

int imageWidth;
int imageHeight;

void DisplayImageAndBox(HDC hdc, const wchar_t* imagePath, int imageWidth, int imageHeight);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

std::string GetLastErrorAsString()
{
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string();
    }

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}


const int main_window_width = 800;
const int main_window_height = 600;

class ImageBoxProxy {
public:
    ImageBoxProxy() : isDragging(false) {
        hBitmap = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1));
        if (hBitmap == NULL) {
            MessageBoxA(GetActiveWindow(), GetLastErrorAsString().c_str(), "Error", MB_ICONERROR);
            return;
        }
    }

    void DisplayBox(HDC hdc, int x, int y) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); 
        SelectObject(hdc, hPen);
        Rectangle(hdc, x, y, imageWidth + x, imageHeight + y);
        DeleteObject(hPen);
    }

    void DisplayImage(HDC hdc, const wchar_t* imagePath, int x, int y)
    {
        HDC hdcMem = CreateCompatibleDC(hdc);
        SelectObject(hdcMem, hBitmap);

        BitBlt(hdc, x, y, imageWidth, imageHeight, hdcMem, 0, 0, SRCCOPY);

        DeleteDC(hdcMem);
    }

    void ButtonDown(POINT actual_point) {
        x -= x_buf;
        y -= y_buf;
        if (actual_point.x >= x && actual_point.x <= x + imageWidth &&
            actual_point.y >= y && actual_point.y <= y + imageHeight / 7 + 10) {
            isDragging = true;
            x_buf = actual_point.x - x;
            y_buf = actual_point.y - y;
            x = actual_point.x;
            y = actual_point.y;
        }
    }

    bool MoveMouse(POINT actual_point) {
        if (isDragging) {
            x = actual_point.x;
            y = actual_point.y;
        }
        return isDragging;
    }

    void Paint(HDC hdc) {
        DisplayBox(hdc, x - x_buf, y - y_buf);
        if (hasImage)
            DisplayImage(hdc, L"TestImage.jpg", x - x_buf, y - y_buf);
    }

    void ButtonUp() {
        isDragging = false;
    }

    void RightDblClick() {
        hasImage = true;
    }

private:
    bool isDragging;
    int x = 0;
    int y = 0;
    int x_buf = 0;
    int y_buf = 0;
    bool hasImage = false;
    HBITMAP hBitmap;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyWindowClass");
    wc.style = CS_DBLCLKS;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, TEXT("MyWindowClass"), TEXT("Простой графический редактор"),
        WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, main_window_width, main_window_height,
        NULL, NULL, hInstance, NULL);
    HDC hdc = GetDC(hwnd);

    std::ifstream file("TestImage.bmp", std::ios::binary);

    BITMAPFILEHEADER bmp_file_header;
    BITMAPINFOHEADER bmp_info_header;

    file.read(reinterpret_cast<char*>(&bmp_file_header), sizeof(BITMAPFILEHEADER));
    file.read(reinterpret_cast<char*>(&bmp_info_header), sizeof(BITMAPINFOHEADER));

    imageWidth = bmp_info_header.biWidth;
    imageHeight = bmp_info_header.biHeight;

    // Главный цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
    }

    ReleaseDC(hwnd, hdc);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static ImageBoxProxy imageBoxProx;

    switch (message) {
    case WM_LBUTTONDOWN:
        imageBoxProx.ButtonDown({ LOWORD(lParam), HIWORD(lParam) });
        SetCapture(hwnd);
        break;
    case WM_MOUSEMOVE:
        if (imageBoxProx.MoveMouse({ LOWORD(lParam), HIWORD(lParam) }))
            InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_LBUTTONUP:
        imageBoxProx.ButtonUp();
        ReleaseCapture();
        break;
    case WM_PAINT:
    {
        FillRect(GetDC(hwnd), new RECT{ 0, 0, main_window_width, main_window_height },
            CreateSolidBrush(RGB(255, 255, 255)));
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        imageBoxProx.Paint(hdc);
        EndPaint(hwnd, &ps);
    }
        break;
    case WM_RBUTTONDBLCLK:
    {
        imageBoxProx.RightDblClick();
        InvalidateRect(hwnd, NULL, TRUE);
    }
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
