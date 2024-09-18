#ifndef IMAGE_HPP
#define IMAGE_HPP

#include<vector>
#include<string>
#include<opencv2/opencv.hpp>

class Image {
    public:
        cv::Mat data;
        int width;
        int height;
        Image();
        Image(cv::Mat data, int width, int height);
        
        cv::Mat getData();
        int getWidth();
        int getHeight();
        void setData(cv::Mat data);
        void setWidth(int width);
        void setHeight(int height);
};

#endif //IMAGE_HPP