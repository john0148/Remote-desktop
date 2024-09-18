#include "imageProcessing.hpp"
#include "net_tsqueue.h"
#include "WindowHandler.hpp"

// these two lines must be included in a CPP file, not in a header file
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// Simple helper function to load an image from file into a OpenGL texture with common settings
bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height) {
    // Load from file
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (image.empty()) {
        return false; // Image loading failed
    }

    // Convert color format
    cv::Mat image_rgba;
    cv::cvtColor(image, image_rgba, cv::COLOR_BGR2RGBA);

    int image_width = image_rgba.cols;
    int image_height = image_rgba.rows;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_rgba.ptr());

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

// Simple helper function to load an image from pixel array into a OpenGL texture with common settings
bool LoadTextureFromImage(const cv::Mat &image, GLuint &out_texture) {
    // Check if the image data is valid
    if (image.empty()) {
        return false; // Image data is invalid
    }

    // Convert color format
    cv::Mat image_rgba;
    cv::cvtColor(image, image_rgba, cv::COLOR_BGR2RGBA);

    glBindTexture(GL_TEXTURE_2D, out_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_rgba.ptr());
    return true;
}

void SendingEvent(Computer *computer) {
    std::pair<WORD, DWORD> keyboardEvent;
    mousePacket mouseEvent;
    while (!computer->socket.bQuit) {
        net::message<CustomMsgTypes> KeyMsg;
        KeyMsg.header.id = CustomMsgTypes::KeyEvent;
        if (computer->socket.kEvents.getKeyboardEvent(keyboardEvent)) {
            std::cout << keyboardEvent.first << ' ' << keyboardEvent.second << '\n';
            std::vector<unsigned char> data(sizeof(WORD) + sizeof(DWORD));
            memcpy(&data[0], &keyboardEvent.first, sizeof(WORD));
            memcpy(&data[sizeof(WORD)], &keyboardEvent.second, sizeof(DWORD));
            KeyMsg << data;
            computer->socket.Send(KeyMsg);
        }

        net::message<CustomMsgTypes> MouseMsg;
        MouseMsg.header.id = CustomMsgTypes::MouseEvent;
        if (computer->socket.mEvents.getMouseEvent(mouseEvent)) {
            std::cout << mouseEvent.header << ' ' << mouseEvent.val
                      << ' ' << mouseEvent.dx << ' ' << mouseEvent.dy << '\n';
            std::vector<unsigned char> data(sizeof(mouseEvent));
            memcpy(&data[0], &mouseEvent, sizeof(mouseEvent));
            MouseMsg << data;
            computer->socket.Send(MouseMsg);
        }
    }
}

void recieveDataOneComputer(Computer *computer) {
    while (!computer->socket.bQuit) {
        if (computer->socket.IsConnected()) {
            if (computer->socket.ImageRequest) {
                // send request to server
                net::message<CustomMsgTypes> msg;
                msg.header.id = CustomMsgTypes::SendingImageRequest;
                computer->socket.Send(msg);
                computer->socket.ImageRequest = false;
            }

            if (!computer->socket.Incoming().empty()) {
                auto msg = computer->socket.Incoming().pop_front().msg;

                switch (msg.header.id) {
                case CustomMsgTypes::ServerAccept:
                    {
                        // Server has responded to a ping request
                        std::cout << "Server Accepted Connection\n";
                    }
                    break;

                    // start transmitting
                    case CustomMsgTypes::ServerScreen:
                    {
                        std::vector<unsigned char> data;
                        msg >> data;
                        computer->socket.frameQueue.addFrame(data, msg.header.width, msg.header.height);
                        std::cout << computer->socket.frameQueue.queue.size() << std::endl;
                    }
                    break;

                case CustomMsgTypes::JoinRequest:
                    {
                        std::cout << "[ join thread share images successfully ]\n";
                        if (computer->socket.serverThread.joinable()) {
                            computer->socket.serverThread.join();
                        }
                    }
                    break;

                case CustomMsgTypes::InitThreadRequest:
                    {
                        computer->socket.running = true;
                    }
                    break;

                    default:
                    {
                        // Handle unknown message type
                        std::cout << "Unknown message type\n";
                    }
                }
            }
        }
        else {
            std::cout << "Server Down\n";
            computer->socket.bQuit = true;
        }
    }
}

void recieveDataAllComputers(std::vector<Computer *> &connected_computers) {
    for (long long unsigned int i = 0; i < connected_computers.size(); i++) {
        recieveDataOneComputer(connected_computers.at(i));
    }
}