#ifndef _NET_IMAGE_H_
#define _NET_IMAGE_H_
#include "net_connection.h"
#include "net_transmit.h"

struct Image {
    int width, height;
    cv::Mat data;
};

class FrameQueueWithLock {
    public:
        void addFrame(const std::vector<unsigned char> &data, const int &width, const int &height) {
            std::vector<unsigned char> decompressData;
            DecompressImage(data, decompressData);
            cv::Mat frame = cv::imdecode(decompressData, cv::IMREAD_UNCHANGED);
            if(!decompressData.empty()) {
                std::unique_lock<std::mutex> lock(queueMutex);
                queue.push(Image{width, height, frame});
                lock.unlock();
            }
        }

        bool getNextFrame(Image &imageData) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (!queue.empty()) {
                    imageData = queue.front();
                    queue.pop();
                    // Mutex sẽ tự động được unlock khi ra khỏi scope này
                }
                else {
                    return false;
                }
            }
            // Ở đây, mutex đã được unlock
            return true;
        }

    private:
        std::queue<Image> queue;
        std::mutex queueMutex;
};

#endif