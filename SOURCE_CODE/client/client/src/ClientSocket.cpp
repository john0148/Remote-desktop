#include "ClientSocket.h"

ClientSocket &ClientSocket::operator=(const ClientSocket &rhs)
{
    if (this != &rhs)
    {
        // Handle simple data members
        this->ImageRequest = rhs.ImageRequest;
        this->bQuit = rhs.bQuit;
        this->running = rhs.running;
        this->mouse_pos_x = rhs.mouse_pos_x;
        this->mouse_pos_y = rhs.mouse_pos_y;

        // Handle complex data members (frameQueue, kEvents, mEvents)
        // This depends on the nature of these classes and how they should be copied

        // Handle the thread
        // You need to decide how to manage the thread. One common approach
        // is to join or detach the existing thread (if running), and then
        // potentially start a new thread if needed.

        // The same goes for any other complex resources like network connections
        // or other system resources that your class might be managing
    }
    return *this;
}
