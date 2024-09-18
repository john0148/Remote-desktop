#ifndef WINDOW_HANDLER_HPP
#define WINDOW_HANDLER_HPP

#include "libs.hpp"

// set window position and size
class WindowHandler {
    private:
        float coorX;
        float coorY;
        float width;
        float height;
    public:
        WindowHandler(float coorX, float coorY, float width, float height);
        float getCoorX();
        float getCoorY();
        float getWidth();
        float getHeight();
        void setWindow();
};

#endif // WINDOW_HANDLER_HPP