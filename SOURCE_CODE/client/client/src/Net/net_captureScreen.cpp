#include "net_captureScreen.h"

HDC globalHdc = NULL;
HDC globalHdcMem = NULL;
HBITMAP globalHBitmap = NULL;
BITMAPINFOHEADER globalBi;
std::vector<unsigned char> globalPixels;
int screenWidth = 0;
int screenHeight = 0;

void InitializeScreenCapture()
{
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    globalHdc = GetDC(NULL);
    globalHdcMem = CreateCompatibleDC(globalHdc);
    globalHBitmap = CreateCompatibleBitmap(globalHdc, screenWidth, screenHeight);
    SelectObject(globalHdcMem, globalHBitmap);

    globalPixels.resize(screenWidth * screenHeight * 3);

    globalBi.biSize = sizeof(BITMAPINFOHEADER);
    globalBi.biWidth = screenWidth;
    globalBi.biHeight = -screenHeight; // negative to flip the image
    globalBi.biPlanes = 1;
    globalBi.biBitCount = 24;
    globalBi.biCompression = BI_RGB;
    globalBi.biSizeImage = 0;
    globalBi.biXPelsPerMeter = 0;
    globalBi.biYPelsPerMeter = 0;
    globalBi.biClrUsed = 0;
    globalBi.biClrImportant = 0;
}

void CaptureScreen(std::vector<unsigned char> &buffer, int &width, int &height)
{
    width = screenWidth;
    height = screenHeight;

    // Perform the bit block transfer
    BitBlt(globalHdcMem, 0, 0, screenWidth, screenHeight, globalHdc, 0, 0, SRCCOPY);

    // Retrieve the bitmap data
    GetDIBits(globalHdcMem, globalHBitmap, 0, screenHeight, globalPixels.data(),
              reinterpret_cast<BITMAPINFO *>(&globalBi), DIB_RGB_COLORS);

    // Prepare the bitmap file header
    BITMAPFILEHEADER bmfh;
    bmfh.bfType = 0x4D42; // 'BM'
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + globalPixels.size();
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Resize buffer and copy data into it
    buffer.resize(bmfh.bfSize);
    memcpy(buffer.data(), &bmfh, sizeof(BITMAPFILEHEADER));
    memcpy(buffer.data() + sizeof(BITMAPFILEHEADER), &globalBi, sizeof(BITMAPINFOHEADER));
    memcpy(buffer.data() + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
           globalPixels.data(), globalPixels.size());
}

void CleanupScreenCapture()
{
    DeleteObject(globalHBitmap);
    DeleteDC(globalHdcMem);
    ReleaseDC(NULL, globalHdc);
}