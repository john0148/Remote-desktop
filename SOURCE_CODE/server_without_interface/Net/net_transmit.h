#ifndef _NET_TRANSMIT_H_
#define _NET_TRANSMIT_H_

#include <windows.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "net_image.h"
#include "process_image_UI.h"

// HDC globalHdc = NULL;
// HDC globalHdcMem = NULL;
// HBITMAP globalHBitmap = NULL;
// BITMAPINFOHEADER globalBi;
// std::vector<unsigned char> globalPixels;
// int screenWidth = 0;
// int screenHeight = 0;

// void InitializeScreenCapture()
// {
    // screenWidth = GetSystemMetrics(SM_CXSCREEN);
    // screenHeight = GetSystemMetrics(SM_CYSCREEN);

//     globalHdc = GetDC(NULL);
//     globalHdcMem = CreateCompatibleDC(globalHdc);
//     globalHBitmap = CreateCompatibleBitmap(globalHdc, screenWidth, screenHeight);
//     SelectObject(globalHdcMem, globalHBitmap);

//     globalPixels.resize(screenWidth * screenHeight * 3);

//     globalBi.biSize = sizeof(BITMAPINFOHEADER);
//     globalBi.biWidth = screenWidth;
//     globalBi.biHeight = -screenHeight; // negative to flip the image
//     globalBi.biPlanes = 1;
//     globalBi.biBitCount = 24;
//     globalBi.biCompression = BI_RGB;
//     globalBi.biSizeImage = 0;
//     globalBi.biXPelsPerMeter = 0;
//     globalBi.biYPelsPerMeter = 0;
//     globalBi.biClrUsed = 0;
//     globalBi.biClrImportant = 0;
// }

// void CaptureScreen(std::vector<unsigned char> &buffer, int &width, int &height)
// {
//     width = screenWidth;
//     height = screenHeight;

//     // Perform the bit block transfer
//     BitBlt(globalHdcMem, 0, 0, screenWidth, screenHeight, globalHdc, 0, 0, SRCCOPY);

//     // Retrieve the bitmap data
//     GetDIBits(globalHdc, globalHBitmap, 0, screenHeight, globalPixels.data(),
//               reinterpret_cast<BITMAPINFO *>(&globalBi), DIB_RGB_COLORS);

//     // Prepare the bitmap file header
//     BITMAPFILEHEADER bmfh;
//     bmfh.bfType = 0x4D42; // 'BM'
//     bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + globalPixels.size();
//     bmfh.bfReserved1 = 0;
//     bmfh.bfReserved2 = 0;
//     bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

//     // Resize buffer and copy data into it
//     buffer.resize(bmfh.bfSize);
//     memcpy(buffer.data(), &bmfh, sizeof(BITMAPFILEHEADER));
//     memcpy(buffer.data() + sizeof(BITMAPFILEHEADER), &globalBi, sizeof(BITMAPINFOHEADER));
//     memcpy(buffer.data() + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
//            globalPixels.data(), globalPixels.size());
// }

// void CleanupScreenCapture()
// {
//     DeleteObject(globalHBitmap);
//     DeleteDC(globalHdcMem);
//     ReleaseDC(NULL, globalHdc);
// }

cv::Mat CaptureScreenMat(HWND hwnd = GetDesktopWindow())
{
    HDC hwindowDC, hwindowCompatibleDC;
    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize; // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom; // change this to whatever size you want to resize to
    width = windowsize.right;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; // this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

void CaptureScreen(std::vector<unsigned char> &buffer, int &width, int &height)
{
    // Capture the screen into a Mat object
    cv::Mat screen = CaptureScreenMat(); // Assuming CaptureScreenMat is implemented as before

    // Check if the capture was successful
    if (screen.empty())
    {
        std::cerr << "Error: Screen capture is empty." << std::endl;
        width = 0;
        height = 0;
        return;
    }

    // Convert Mat to vector<unsigned char> using imencode
    if (!cv::imencode(".bmp", screen, buffer))
    {
        std::cerr << "Error: Could not encode captured screen to buffer." << std::endl;
        width = 0;
        height = 0;
        return;
    }

    // Set width and height
    width = screen.cols;
    height = screen.rows;
}

void CompressImage(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer) {
    try
    {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat inputImage = cv::imdecode(inputBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (inputImage.empty())
        {   
            std::cerr << "Cannot tranfer image data\n";
            return;
        }

        // Compress and save to vector<uchar> compressedBuffer
        cv::imencode(".jpg", inputImage, compressedBuffer);
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void CompressImageTo480p(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer, int quality = 30) {
    try {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat inputImage = cv::imdecode(inputBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (inputImage.empty())
        {
            std::cerr << "Không thể chuyển đổi dữ liệu ảnh\n";
            return;
        }

        // Thay đổi kích thước ảnh xuống còn 480p (640x480)
        cv::Mat resizedImage;
        cv::resize(inputImage, resizedImage, cv::Size(640, 480));

        // Cấu hình tham số nén
        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(quality);

        // Compress and save to vector<uchar> compressedBuffer
        cv::imencode(".jpg", resizedImage, compressedBuffer, compression_params);
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void CompressImageTo600p(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer, int quality = 60) {
    try
    {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat inputImage = cv::imdecode(inputBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (inputImage.empty())
        {
            std::cerr << "Không thể chuyển đổi dữ liệu ảnh\n";
            return;
        }

        // Thay đổi kích thước ảnh xuống còn 600p (800x600)
        cv::Mat resizedImage;
        cv::resize(inputImage, resizedImage, cv::Size(800, 600));

        // Cấu hình tham số nén
        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(quality);

        // Compress and save to vector<uchar> compressedBuffer
        cv::imencode(".jpg", resizedImage, compressedBuffer, compression_params);
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}


void DecompressImage(const std::vector<unsigned char> &compressedBuffer, std::vector<unsigned char> &bmpBuffer) {
    try {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat decompressedImage = cv::imdecode(compressedBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (decompressedImage.empty()) {
            std::cerr << "Không thể giải nén dữ liệu ảnh\n";
            return;
        }

        cv::imencode(".bmp", decompressedImage, bmpBuffer);
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void ImageToTexture(std::vector<unsigned char> &imageData, GLuint& textureID, int& width, int& height, int channels) {

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

    glBindTexture(GL_TEXTURE_2D, 0);
}


#endif