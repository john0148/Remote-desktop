#include "WindowHandler.hpp"

WindowHandler::WindowHandler(float coorX, float coorY, float width, float height) {
    if (coorX < 0) coorX = 0;
    if (coorY < 0) coorY = 0;
    if (width < 0) width = 0;
    if (height < 0) height = 0;
    
    this->coorX = coorX;
    this->coorY = coorY;
    this->width = width;
    this->height = height;
}

float WindowHandler::getCoorX() {
    return this->coorX;
}

float WindowHandler::getCoorY() {
    return this->coorY;
}

float WindowHandler::getWidth() {
    return this->width;
}

float WindowHandler::getHeight() {
    return this->height;
}

void WindowHandler::setWindow() {
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + this->getCoorX(), main_viewport->WorkPos.y + this->getCoorY()), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(this->getWidth(), this->getHeight()), ImGuiCond_None);
}