#ifndef _NET_IMAGE_H_
#define _NET_IMAGE_H_
#include "net_connection.h"
#include <opencv2/opencv.hpp>
#include "net_transmit.h"
#include "../Image.hpp"

class FrameQueueWithLock
{
public:
    void addFrame(const std::vector<unsigned char> &data, const int &width, const int &height)
    {
        std::vector<unsigned char> decompressData;
        DecompressImage(data, decompressData);
        cv::Mat frame = cv::imdecode(decompressData, cv::IMREAD_UNCHANGED);
        if (!decompressData.empty())
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (queue.size() > 1)
                queue.pop_front();
            queue.push_back(Image{frame, width, height});
            lock.unlock();
        }
    }

    bool getFrameBack(Image &image)
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (!queue.empty())
            {
                image = queue.back();
                // Mutex sẽ tự động được unlock khi ra khỏi scope này
            }
            else
            {
                return false;
            }
        }
        // Ở đây, mutex đã được unlock
        return true;
    }

    bool getNextFrame(Image &image)
    {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (!queue.empty())
            {
                image = queue.front();
                queue.pop_front();
                // Mutex sẽ tự động được unlock khi ra khỏi scope này
            }
            else
            {
                return false;
            }
        }
        // Ở đây, mutex đã được unlock
        return true;
    }

public:
    std::deque<Image> queue;
    std::mutex queueMutex;
};

#endif