#pragma once

#include "olc_net.h"
#include "process_image_UI.h"

class ClientSocket : public net::client_interface<CustomMsgTypes>
{
public:
    ClientSocket &operator=(const ClientSocket &rhs);

public:
    bool ImageRequest = true;
    bool bQuit = false;

    // using to process image requests
    FrameQueueWithLock frameQueue;
    std::thread serverThread;

    // using for processing image data
    bool running = false;

    // using for processing keyboard and cursor events
    keyEvents kEvents;
    mouseEvents mEvents;

    float mouse_pos_x;
    float mouse_pos_y;
};
