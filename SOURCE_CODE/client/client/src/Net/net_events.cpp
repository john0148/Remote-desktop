#include "net_events.h"

std::unordered_map<SDL_Keycode, WORD> convertSDLtoWIN = {
    {SDLK_BACKSPACE, VK_BACK},
    {SDLK_TAB, VK_TAB},
    {SDLK_RETURN, VK_RETURN},
    {SDLK_ESCAPE, VK_ESCAPE},
    {SDLK_SPACE, VK_SPACE},
    {SDLK_EXCLAIM, '1'},
    {SDLK_QUOTEDBL, '2'},
    {SDLK_HASH, '3'},
    {SDLK_DOLLAR, '4'},
    {SDLK_PERCENT, '5'},
    {SDLK_AMPERSAND, '6'},
    {SDLK_QUOTE, VK_OEM_7}, // ' key
    {SDLK_LEFTPAREN, '9'},
    {SDLK_RIGHTPAREN, '0'},
    {SDLK_ASTERISK, VK_MULTIPLY},
    {SDLK_PLUS, VK_ADD},
    {SDLK_COMMA, VK_OEM_COMMA},
    {SDLK_MINUS, VK_OEM_MINUS},
    {SDLK_PERIOD, VK_OEM_PERIOD},
    {SDLK_SLASH, VK_OEM_2}, // / key
    {SDLK_0, '0'},
    {SDLK_1, '1'},
    {SDLK_2, '2'},
    {SDLK_3, '3'},
    {SDLK_4, '4'},
    {SDLK_5, '5'},
    {SDLK_6, '6'},
    {SDLK_7, '7'},
    {SDLK_8, '8'},
    {SDLK_9, '9'},
    {SDLK_COLON, VK_OEM_1},     // : key
    {SDLK_SEMICOLON, VK_OEM_1}, // ; key
    {SDLK_LESS, VK_OEM_COMMA},  // , key
    {SDLK_EQUALS, VK_OEM_PLUS},
    {SDLK_GREATER, VK_OEM_PERIOD},   // . key
    {SDLK_QUESTION, VK_OEM_2},       // ? key
    {SDLK_AT, VK_OEM_3},             // @ key
    {SDLK_LEFTBRACKET, VK_OEM_4},    // [ key
    {SDLK_BACKSLASH, VK_OEM_5},      // \ key
    {SDLK_RIGHTBRACKET, VK_OEM_6},   // ] key
    {SDLK_CARET, VK_OEM_7},          // ^ key
    {SDLK_UNDERSCORE, VK_OEM_MINUS}, // _ key
    {SDLK_BACKQUOTE, VK_OEM_3},      // ` key
    {SDLK_a, 'A'},
    {SDLK_b, 'B'},
    {SDLK_c, 'C'},
    {SDLK_d, 'D'},
    {SDLK_e, 'E'},
    {SDLK_f, 'F'},
    {SDLK_g, 'G'},
    {SDLK_h, 'H'},
    {SDLK_i, 'I'},
    {SDLK_j, 'J'},
    {SDLK_k, 'K'},
    {SDLK_l, 'L'},
    {SDLK_m, 'M'},
    {SDLK_n, 'N'},
    {SDLK_o, 'O'},
    {SDLK_p, 'P'},
    {SDLK_q, 'Q'},
    {SDLK_r, 'R'},
    {SDLK_s, 'S'},
    {SDLK_t, 'T'},
    {SDLK_u, 'U'},
    {SDLK_v, 'V'},
    {SDLK_w, 'W'},
    {SDLK_x, 'X'},
    {SDLK_y, 'Y'},
    {SDLK_z, 'Z'},
    {SDLK_DELETE, VK_DELETE},
    {SDLK_KP_PERIOD, VK_DECIMAL},
    {SDLK_KP_DIVIDE, VK_DIVIDE},
    {SDLK_KP_MULTIPLY, VK_MULTIPLY},
    {SDLK_KP_MINUS, VK_SUBTRACT},
    {SDLK_KP_PLUS, VK_ADD},
    {SDLK_KP_ENTER, VK_RETURN},
    {SDLK_KP_EQUALS, VK_OEM_NEC_EQUAL},
    {SDLK_UP, VK_UP},
    {SDLK_DOWN, VK_DOWN},
    {SDLK_RIGHT, VK_RIGHT},
    {SDLK_LEFT, VK_LEFT},
    {SDLK_INSERT, VK_INSERT},
    {SDLK_HOME, VK_HOME},
    {SDLK_END, VK_END},
    {SDLK_PAGEUP, VK_PRIOR},
    {SDLK_PAGEDOWN, VK_NEXT},
    {SDLK_F1, VK_F1},
    {SDLK_F2, VK_F2},
    {SDLK_F3, VK_F3},
    {SDLK_F4, VK_F4},
    {SDLK_F5, VK_F5},
    {SDLK_F6, VK_F6},
    {SDLK_F7, VK_F7},
    {SDLK_F8, VK_F8},
    {SDLK_F9, VK_F9},
    {SDLK_F10, VK_F10},
    {SDLK_F11, VK_F11},
    {SDLK_F12, VK_F12},
    {SDLK_NUMLOCKCLEAR, VK_NUMLOCK},
    {SDLK_CAPSLOCK, VK_CAPITAL},
    {SDLK_SCROLLLOCK, VK_SCROLL},
    {SDLK_RSHIFT, VK_RSHIFT},
    {SDLK_LSHIFT, VK_LSHIFT},
    {SDLK_RCTRL, VK_RCONTROL},
    {SDLK_LCTRL, VK_LCONTROL},
    {SDLK_RALT, VK_RMENU},
    {SDLK_LALT, VK_LMENU},
    {SDLK_RGUI, VK_RWIN},
    {SDLK_LGUI, VK_LWIN}};

void keyEvents::addKeydownEvent(const WORD &winKeycode)
{
    std::unique_lock<std::mutex> lock(keyboardMutex);
    keyboardEvents.push(std::make_pair(winKeycode, 0));
    lock.unlock();
}

void keyEvents::addKeyupEvent(const WORD &winKeycode)
{
    std::unique_lock<std::mutex> lock(keyboardMutex);
    keyboardEvents.push(std::make_pair(winKeycode, KEYEVENTF_KEYUP));
    lock.unlock();
}

bool keyEvents::getKeyboardEvent(std::pair<WORD, DWORD> &obj)
{
    std::unique_lock<std::mutex> lock(keyboardMutex);
    if (!keyboardEvents.empty())
    {
        obj = keyboardEvents.front();
        keyboardEvents.pop();
        return true;
    }
    return false;
}

// Implementations for mousePacket
mousePacket::mousePacket(short _header, int32_t _val, double _dx, double _dy)
    : header(_header), val(_val), dx(_dx), dy(_dy)
{
}

// Implementations for mouseEvents
void mouseEvents::addMouseEvent(const int32_t &type)
{
    std::unique_lock<std::mutex> lock(mousedMutex);
    mouseEvents.push(mousePacket(0, type));
    lock.unlock();
}

void mouseEvents::addMouseMotion(const float &x, const float &y)
{
    std::unique_lock<std::mutex> lock(mousedMutex);
    mouseEvents.push(mousePacket(1, 0, x, y));
    lock.unlock();
}

void mouseEvents::addMouseWheel(const int32_t &len)
{
    std::unique_lock<std::mutex> lock(mousedMutex);
    mouseEvents.push(mousePacket(2, len));
    lock.unlock();
}

bool mouseEvents::getMouseEvent(mousePacket &obj)
{
    std::unique_lock<std::mutex> lock(mousedMutex);
    if (!mouseEvents.empty())
    {
        obj = mouseEvents.front();
        mouseEvents.pop();
        return true;
    }
    return false;
}