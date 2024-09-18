#include "mainFrame.hpp"

void mainFrame() {
    static std::vector<Computer*> connected_computers;
    static bool init_connected_computers = false;
    if (!init_connected_computers) {
        connected_computers.reserve(4);
        init_connected_computers = true;
    }
    static std::vector<Image> images;
    TopMenuBar topMenuBar;
    topMenuBar.showTopMenuBar(connected_computers, images);
}