#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include "libs.hpp"
#include "Computer.hpp"
#include "WindowHandler.hpp"

bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);
bool LoadTextureFromImage(const cv::Mat &image, GLuint &out_texture);
void SendingEvent(Computer *computer);
void recieveDataOneComputer(Computer *computer);
void recieveDataAllComputers(std::vector<Computer *> &connected_computers);

#endif // IMAGE_PROCESSING_HPP