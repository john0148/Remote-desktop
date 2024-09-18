#ifndef TOP_MENU_BAR_HPP
#define TOP_MENU_BAR_HPP

#include "WindowHandler.hpp"
#include "imageProcessing.hpp"

extern bool isThreadCreated;
extern Computer *controllingCom;
extern bool isControllingComputer;
extern HHOOK g_hKeyboardHook;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

class TopMenuBar
{
private:
    void showMenuConnect(std::vector<Computer *> &connected_computers, std::vector<Image> &images);
    void showMenuControl(std::vector<Computer *> &connected_computers, std::vector<Image> &images);

public:
    void showTopMenuBar(std::vector<Computer *> &connected_computers, std::vector<Image> &images);
    void connectNewComputer(std::vector<Computer *> &connected_computers, std::vector<Image> &images, bool &isOpeningConnectBox);
    std::thread serThread;
};

// connectComputer

// --------- begin define functions--------- //

void showConnectedComputer(std::vector<Computer *> &connected_computers, bool &isShowingConnectedComputers);
void disconnectComputer(std::vector<Computer *> &connected_computers, std::vector<Image> &images, long long unsigned int pos);
void disconnectAllComputers(std::vector<Computer *> &connected_computers, std::vector<Image> &images);

// --------- finish define functions--------- //

// controlComputer

// --------- begin define functions--------- //

void viewOneComputerInViewAllMode(Computer *computer, WindowHandler &windowHandler, const GLuint &my_image_texture, bool &isViewingAllComputers);
void viewAllComputers(std::vector<Computer *> &connected_computers, std::vector<Image> &images, bool &isViewingAllComputers);
void controllingComputer(Computer *computer, WindowHandler &windowHandler, const GLuint &my_image_texture);
void controlComputer(Computer *computer);

// --------- finish define functions--------- //

#endif // TOP_MENU_BAR_HPP