#include "Computer.hpp"

Computer::Computer() {
    this->name = "Undefined";
    this->ip = "127.0.0.1";
    this->port = "0";
}

Computer::Computer(std::string name, std::string ip, std::string port) {
    Computer::setName(name);
    Computer::setIP(ip);
    Computer::setPort(port);
}

Computer::Computer(const Computer &computer) {
    this->name = computer.name;
    this->ip = computer.ip;
    this->port = computer.port;
    this->socket = computer.socket;
}

std::string Computer::getName() {
    return this->name;
}

std::string Computer::getIP() {
    return this->ip;
}

std::string Computer::getPort() {
    return this->port;
}

void Computer::setName(std::string name) {
    if (name.size() == 0) name = "Undefined";
    this->name = name;
}

void Computer::setIP(std::string ip) {
    if (ip.length() > 15) {
        ip = "127.0.0.1";
    }
    this->ip = ip;
}

void Computer::setPort(std::string port) {
    if (port.length() > 6) {
        port = "0";
    }
    this->port = port;
}

Computer::~Computer() {
    this->socket.Disconnect();
}