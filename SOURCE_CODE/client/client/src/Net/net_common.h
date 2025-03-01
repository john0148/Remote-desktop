#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <queue>
#include <utility>
#include <unordered_map>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

// #ifdef _WIN32
// #define _WIN32_WINNT 0x0A00
// #endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <buffer.hpp>
#include <internet.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace asio = boost::asio;

enum class CustomMsgTypes : uint32_t {
    ServerAccept,
    ServerDeny,
    ServerScreen,
    MessageAll,
    ServerMessage,
    StopSendingImage,
    JoinRequest,
    SendingImageRequest,
    InitThreadRequest,
    KeyEvent,
    MouseEvent,
    FinishRequest,
};