#ifndef COMPUTER_HPP
#define COMPUTER_HPP

#include<string>

#include "ClientSocket.h"

class Computer {
    private:
        std::string name;
        std::string ip;
        std::string port;
    public:
        ClientSocket socket;
        
        Computer();
        Computer(std::string name, std::string ip, std::string port);
        Computer(const Computer &computer);
        void setName(std::string name);
        void setIP(std::string ip);
        void setPort(std::string port);
        std::string getName();
        std::string getIP();
        std::string getPort();
        ~Computer();
};

#endif //COMPUTER_HPP