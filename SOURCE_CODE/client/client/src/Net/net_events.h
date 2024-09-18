#ifndef _NET_EVENTS_H_
#define _NET_EVENTS_H_
#include "net_common.h"
#include "process_image_UI.h"

extern std::unordered_map<SDL_Keycode, WORD> convertSDLtoWIN;

class keyEvents
{
public:
    void addKeydownEvent(const WORD &winKeycode);
    void addKeyupEvent(const WORD &winKeycode);
    bool getKeyboardEvent(std::pair<WORD, DWORD> &obj);

private:
    std::mutex keyboardMutex;
    std::queue<std::pair<WORD, DWORD>> keyboardEvents;
};

#pragma pack(push, 1)
class mousePacket
{
public:
    short header;
    int32_t val;
    double dx, dy;

    mousePacket(short _header = 0, int32_t _val = 0, double _dx = 0, double _dy = 0);
};
#pragma pack(pop)

class mouseEvents
{
public:
    void addMouseEvent(const int32_t &type);
    void addMouseMotion(const float &x, const float &y);
    void addMouseWheel(const int32_t &len);
    bool getMouseEvent(mousePacket &obj);

private:
    std::mutex mousedMutex;
    std::queue<mousePacket> mouseEvents;
};

#endif