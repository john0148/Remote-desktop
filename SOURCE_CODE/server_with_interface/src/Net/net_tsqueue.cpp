#include "net_tsqueue.h"

namespace net {
    template<typename T>
    tsqueue<T>::tsqueue() = default;

    template<typename T>
    tsqueue<T>::~tsqueue() {
        clear();
    }

    template<typename T>
    const T& tsqueue<T>::front() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }

    template<typename T>
    const T& tsqueue<T>::back() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }

    template<typename T>
    T tsqueue<T>::pop_front() {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }

    template<typename T>
    T tsqueue<T>::pop_back() {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.back());
        deqQueue.pop_back();
        return t;
    }

    template<typename T>
    void tsqueue<T>::push_back(const T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }

    template<typename T>
    void tsqueue<T>::push_front(const T& item) {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }

    template<typename T>
    bool tsqueue<T>::empty() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }

    template<typename T>
    size_t tsqueue<T>::count() {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }

    template<typename T>
    void tsqueue<T>::clear() {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }

    template<typename T>
    void tsqueue<T>::wait() {
        while (empty()) {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
        }
    }
}