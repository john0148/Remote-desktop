#include "TopMenuBar.hpp"
#include "clientSocket.h"


// bool isControllingComputer = false;
bool isThreadCreated = false;
Computer *controllingCom = nullptr;
bool isControllingComputer = false;
std::thread ThreadDataServer;
std::thread ThreadEventServer;

HHOOK g_hKeyboardHook = nullptr;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        // In ra thông tin về sự kiện bàn phím (không thực thi)
        if (isControllingComputer && isThreadCreated) {
            KBDLLHOOKSTRUCT* pKeyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            // std::cout << "Key pressed: " << pKeyboardStruct->vkCode << ' ' << pKeyboardStruct->flags<< std::endl;
            if (pKeyboardStruct->flags < 128) controllingCom->socket.kEvents.addKeydownEvent(pKeyboardStruct->vkCode);
            else controllingCom->socket.kEvents.addKeyupEvent(pKeyboardStruct->vkCode);
        }
        return 1;
    }

    // Tiếp tục xử lý sự kiện
    return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

void TopMenuBar::showTopMenuBar(std::vector<Computer *> &connected_computers, std::vector<Image> &images) {

    ImGui::BeginMainMenuBar();
    {
        this->showMenuConnect(connected_computers, images);
        this->showMenuControl(connected_computers, images);
    }
    ImGui::EndMainMenuBar();
}

void TopMenuBar::showMenuConnect(std::vector<Computer *> &connected_computers, std::vector<Image> &images) {
    static bool isOpenConnectBox = false;
    static bool isShowConnectedComputers = false;

    if (ImGui::BeginMenu("Connect")) {
        ImGui::MenuItem("Connect new computer", nullptr, &isOpenConnectBox);
        (connected_computers.size() > 0) && (ImGui::MenuItem("Show connected computers", nullptr, &isShowConnectedComputers));

        if (connected_computers.size() != 0 && ImGui::BeginMenu("Disconnect computer")) {
            for (long long unsigned int i = 0; i < connected_computers.size(); ++i) {
                if (ImGui::MenuItem(connected_computers.at(i)->getName().c_str())) {
                    disconnectComputer(connected_computers, images, i);
                }
            }
            ImGui::EndMenu();
        }

        if (connected_computers.size() > 1 && ImGui::MenuItem("Disconnect all computers")) {
            disconnectAllComputers(connected_computers, images);
        }

        ImGui::EndMenu();
    }

    if (isOpenConnectBox) {
        connectNewComputer(connected_computers, images, isOpenConnectBox);
    }

    if (isShowConnectedComputers) {
        showConnectedComputer(connected_computers, isShowConnectedComputers);
    }
}

void TopMenuBar::showMenuControl(std::vector<Computer *> &connected_computers, std::vector<Image> &images) {
    static bool isViewingAllComputers = false;
    static Image controllingImage;

    if (ImGui::BeginMenu("Control")) {
        for (long long unsigned int i = 0; i < connected_computers.size(); ++i) {
            // if (computer->socket.m_connection->IsSocketOpen())
            if (!connected_computers.at(i)->socket.IsConnected()) {
                disconnectComputer(connected_computers, images, i);
            }
        }
        
        if (ImGui::MenuItem("View all computers", nullptr, &isViewingAllComputers)) {
            // turn off control mode
            isControllingComputer = false;
        }

        if (connected_computers.size() > 0 && ImGui::BeginMenu("Control computer")) {
            for (long long unsigned int i = 0; i < connected_computers.size(); ++i) {
                if (ImGui::MenuItem(connected_computers.at(i)->getName().c_str())) {
                    // switch to control mode
                    isViewingAllComputers = false;
                    isControllingComputer = true;

                    controllingCom = connected_computers.at(i);
                    controllingImage = images.at(i);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }

    if (isViewingAllComputers) {
        viewAllComputers(connected_computers, images, isViewingAllComputers);
    }

    if (isControllingComputer && isThreadCreated == false) {
        isThreadCreated = true;

        controllingCom->socket.bQuit = false;
        controllingCom->socket.ImageRequest = true;

        ThreadDataServer = std::thread(recieveDataOneComputer, std::ref(controllingCom));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        ThreadEventServer = std::thread(SendingEvent, std::ref(controllingCom));
        g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, GetModuleHandle(NULL), 0);
    }

    if (isControllingComputer && isThreadCreated == true) {
        controlComputer(controllingCom);
    }

    if (isControllingComputer == false && isThreadCreated == true) {
        isThreadCreated = false;
        // send request to server to stop sending image
        net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::StopSendingImage;
        controllingCom->socket.m_connection->Send(msg);

        controllingCom->socket.bQuit = true;

        if (ThreadDataServer.joinable()) {
            ThreadDataServer.join();
        }

        if (g_hKeyboardHook != nullptr) {
            UnhookWindowsHookEx(g_hKeyboardHook);
            g_hKeyboardHook = nullptr;
        }

        if (ThreadEventServer.joinable()) {
            ThreadEventServer.join();
        }
    }
}

// connectComputer

void TopMenuBar::connectNewComputer(std::vector<Computer *> &connected_computers, std::vector<Image> &images, bool &isOpeningConnectBox) {
    WindowHandler windowHandler(50.0, 60.0, 300.0, 300.0);
    windowHandler.setWindow();

    static char name[50];
    static char ip[20];
    static char port[7];

    static bool isFirstOpenConnectBox = true;
    if (isFirstOpenConnectBox) {
        isFirstOpenConnectBox = false;
        memset(name, 0, sizeof(name));
        memset(ip, 0, sizeof(ip));
        memset(port, 0, sizeof(port));
    }

    ImGui::Begin("Connect to computer");
    ImGui::InputText("Name", name, 49);
    ImGui::InputText("IP", ip, 19);
    ImGui::InputText("Port", port, 6);

    if (ImGui::Button("Connect")) {
        Computer *computer = new Computer;
        computer->setName(name);
        computer->setIP(ip);
        computer->setPort(port);

        // CONNECT SOCKET
        computer->socket.Connect(ip, std::stoi(port));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (computer->socket.m_connection->IsSocketOpen()) {
            std::cout << "initialized thread successfully" << std::endl;
            connected_computers.push_back(computer);
            isOpeningConnectBox = false;
            Image image;
            images.push_back(image);
        }
    }
    ImGui::End();
}

void showConnectedComputer(std::vector<Computer *> &connected_computers, bool &isShowingConnectedComputers) {
    WindowHandler windowHandler(50.0, 60.0, 500.0, 300.0);
    windowHandler.setWindow();

    ImGui::Begin("Computers info");
    if (ImGui::Button("Close")) {
        isShowingConnectedComputers = false;
    }
    if (connected_computers.size() > 0) {
        for (long long unsigned int i = 0; i < connected_computers.size(); ++i) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Computer %llu", i + 1);
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Name");
            ImGui::SameLine();
            ImGui::Text(connected_computers.at(i)->getName().c_str());
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "IP");
            ImGui::SameLine();
            ImGui::Text(connected_computers.at(i)->getIP().c_str());
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Port");
            ImGui::SameLine();
            ImGui::Text(connected_computers.at(i)->getPort().c_str());
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "---------------------------");
        }
    }
    else {
        ImGui::Text("No computer connected");
    }
    ImGui::End();
}

void disconnectComputer(std::vector<Computer *> &connected_computers, std::vector<Image> &images, long long unsigned int pos) {
    Computer* disconnect = connected_computers.at(pos);
    if(disconnect == controllingCom && isThreadCreated && isControllingComputer){
        isControllingComputer = false;
        isThreadCreated = false;
        // send request to server to stop sending image
        net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::StopSendingImage;
        disconnect->socket.m_connection->Send(msg);

        disconnect->socket.bQuit = true;

        if (ThreadDataServer.joinable()) {
            ThreadDataServer.join();
        }

        if (g_hKeyboardHook != nullptr) {
            UnhookWindowsHookEx(g_hKeyboardHook);
            g_hKeyboardHook = nullptr;
        }

        if (ThreadEventServer.joinable()) {
            ThreadEventServer.join();
        }
    }
    
    connected_computers.at(pos)->socket.Disconnect();
    delete connected_computers.at(pos);
    connected_computers.at(pos) = nullptr;
    connected_computers.erase(connected_computers.begin() + pos);
    images.erase(images.begin() + pos);
}

void disconnectAllComputers(std::vector<Computer *> &connected_computers, std::vector<Image> &images) {
    for (long long unsigned int i = 0; i < connected_computers.size(); ++i) {
        disconnectComputer(connected_computers, images, i);
    }
}

void viewOneComputerInViewAllMode(Computer *computer, WindowHandler &windowHandler, const GLuint &my_image_texture, bool &isViewingAllComputers) {
    windowHandler.setWindow();
    ImGui::BeginChildFrame(ImGui::GetID(computer->getName().c_str()), ImVec2(windowHandler.getWidth(), windowHandler.getHeight()));
    if (ImGui::IsWindowFocused()) {
        isControllingComputer = isThreadCreated = true;
        isViewingAllComputers = false;
        controllingCom = computer;
    }
    ImGui::Text("Name: %s", computer->getName().c_str());
    ImGui::Image((void *)(intptr_t)my_image_texture, ImVec2(windowHandler.getWidth(), windowHandler.getHeight() - 30));
    ImGui::EndChildFrame();
}

void viewAllComputers(std::vector<Computer *> &connected_computers, std::vector<Image> &images, bool &isViewingAllComputers) {
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    LoadTextureFromFile("img/windows-11-logo.bmp", &my_image_texture, &my_image_width, &my_image_height);

    WindowHandler windowHandler(0, 0, ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y);
    windowHandler.setWindow();

    long long unsigned int numberOfSubWindowsInLine = 4;
    float subWindowWidth = 300;
    float subWindowHeight = 200;
    float subWindowCoorX = 0;
    float subWindowCoorY = 0;
    float spaceBetweenFramesInLine = (ImGui::GetMainViewport()->Size.x - subWindowWidth * numberOfSubWindowsInLine) / (numberOfSubWindowsInLine + 1);
    float spaceBetweenFramesInColumn = 50;

    ImGui::Begin("Connected computers");

    if (ImGui::Button("Close")) {
        isViewingAllComputers = false;
    }

    for (long long unsigned int i = 0; i < connected_computers.size(); ++i) {
        subWindowCoorX = spaceBetweenFramesInLine + (i % numberOfSubWindowsInLine) * (spaceBetweenFramesInLine + subWindowWidth);
        subWindowCoorY = spaceBetweenFramesInColumn + (i / numberOfSubWindowsInLine) * (spaceBetweenFramesInColumn + subWindowHeight);
        WindowHandler subWindowHandler(subWindowCoorX, subWindowCoorY, subWindowWidth, subWindowHeight);

        viewOneComputerInViewAllMode(connected_computers.at(i), subWindowHandler, my_image_texture, isViewingAllComputers);

        if ((i % numberOfSubWindowsInLine) != numberOfSubWindowsInLine - 1) {
            ImGui::SameLine();
        }
    }

    ImGui::End();
}

void controllingComputer(Computer *computer, WindowHandler &windowHandler, const GLuint &my_image_texture) {
    const float BLUE_BAR_SIZE = 30;
    windowHandler.setWindow();
    ImGui::BeginChildFrame(ImGui::GetID(computer->getName().c_str()), ImVec2(windowHandler.getWidth(), windowHandler.getHeight()));
    ImGui::GetCursorPos();
    computer->socket.mouse_pos_x = (ImGui::GetMousePos().x - windowHandler.getCoorX()) / ImGui::GetWindowSize().x;
    computer->socket.mouse_pos_y = (ImGui::GetMousePos().y - windowHandler.getCoorY() - BLUE_BAR_SIZE + 10) / ImGui::GetWindowSize().y;
    if (computer->socket.mouse_pos_x < 0) {
        computer->socket.mouse_pos_x = 0;
    }
    else if (computer->socket.mouse_pos_x > 1) {
        computer->socket.mouse_pos_x = 1;
    }
    if (computer->socket.mouse_pos_y < 0)
    {
        computer->socket.mouse_pos_y = 0;
    }
    else if (computer->socket.mouse_pos_y > 1)
    {
        computer->socket.mouse_pos_y = 1;
    }
    ImGui::Image((void *)(intptr_t)my_image_texture, ImVec2(windowHandler.getWidth(), windowHandler.getHeight() - 10)); // -10 to prevent scroll bar
    ImGui::EndChildFrame();
}

void controlComputer(Computer *computer) {
    // initialize texture
    static GLuint my_image_texture;
    static bool initTexture = false;
    if (!initTexture) {
        glGenTextures(1, &my_image_texture);
        initTexture = true;
    }
    int my_image_width = 0;
    int my_image_height = 0;

    static Image image;
    if (computer->socket.frameQueue.getFrameBack(image)) {
        std::string subWindowName = "Computer " + computer->getName();

        WindowHandler windowHandler(0, 0, ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y);
        windowHandler.setWindow();
        ImGui::Begin(subWindowName.c_str());

        if (ImGui::Button("Close")) {
            computer->socket.running = false;
            isControllingComputer = false;
            std::cout << "Control stop\n";
        }
        my_image_width = image.width;
        my_image_height = image.height;
        LoadTextureFromImage(image.data, my_image_texture);

        float subWindowHeight = ImGui::GetMainViewport()->Size.y - 80;
        float subWindowWidth = (float)my_image_width / my_image_height * subWindowHeight;
        float subWindowCoorX = (ImGui::GetMainViewport()->Size.x - subWindowWidth) / 2;
        float subWindowCoorY = 50;

        WindowHandler subWindowHandler(subWindowCoorX, subWindowCoorY, subWindowWidth, subWindowHeight);
        controllingComputer(computer, subWindowHandler, my_image_texture);

        ImGui::End();
    }
    else {
        // Thêm độ trễ để giảm sử dụng CPU khi không có hình ảnh mới
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}