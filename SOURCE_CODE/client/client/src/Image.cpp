#include "Image.hpp"

Image::Image() {
    this->width = 0;
    this->height = 0;
}

Image::Image(cv::Mat data, int width, int height) {
    this->data = data;
    this->width = width;
    this->height = height;
}

cv::Mat Image::getData() {
    return this->data;
}

int Image::getWidth() {
    return this->width;
}

int Image::getHeight() {
    return this->height;
}

void Image::setData(cv::Mat data) {
    this->data = data;
}

void Image::setWidth(int width) {
    this->width = width;
}

void Image::setHeight(int height) {
    this->height = height;
}