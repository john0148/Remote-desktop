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

// Concurrent map structure
// class ConcurrentFrameMap {
//     public:
//         void addFrameToClientQueue(const uint32_t clientID, const std::vector<unsigned char> &data, const int &width, const int &height) {
//             frameQueues[clientID].addFrame(data, width, height);
//         }

//         bool getNextFrameFromClientQueue(const uint32_t clientID, Image &imageData) {
//             return frameQueues[clientID].getNextFrame(imageData);
//         }

//         void removeClient(const uint32_t clientID) {
//             if (frameQueues.find(clientID) != frameQueues.end()) {
//                 frameQueues.erase(clientID);
//             }
//         }

//     private:
//         std::unordered_map<uint32_t, FrameQueueWithLock> frameQueues;
// };

#endif