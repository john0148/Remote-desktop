#ifndef _NET_CAPTURESCREEN_H_
#define _NET_CAPTURESCREEN_H_
#include "net_common.h"
#include<windows.h>

extern HDC globalHdc;
extern HDC globalHdcMem;
extern HBITMAP globalHBitmap;
extern BITMAPINFOHEADER globalBi;
extern std::vector<unsigned char> globalPixels;
extern int screenWidth;
extern int screenHeight;

void InitializeScreenCapture();
void CaptureScreen(std::vector<unsigned char> &buffer, int &width, int &height);
void CleanupScreenCapture();

#endif