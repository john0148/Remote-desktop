#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "mainFrame.hpp"

// --------- begin define functions--------- //

void setupSDL();
void setupImGui(SDL_Window *window, SDL_GLContext gl_context);
void eventProcessing(Computer *computer, SDL_Event &event);
void handleEvents(SDL_Window *window, bool &done);
void updateGUI();
void renderGUI(SDL_Window *window);
void cleanup(SDL_Window *window, SDL_GLContext gl_context);

// --------- finish define functions--------- //

#endif // CLIENT_HPP