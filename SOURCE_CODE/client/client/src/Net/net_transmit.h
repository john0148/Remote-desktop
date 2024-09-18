#ifndef _NET_TRANSMIT_H_
#define _NET_TRANSMIT_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>

void CompressImage(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer);
void CompressImageTo480p(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer, int quality = 30);
void CompressImageTo600p(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer, int quality = 60);
void DecompressImage(const std::vector<unsigned char> &compressedBuffer, std::vector<unsigned char> &bmpBuffer);

#endif