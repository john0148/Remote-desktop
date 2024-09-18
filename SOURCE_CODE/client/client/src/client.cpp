#include "client.hpp"

// Main code
int main(int, char **)
{
    setupSDL();

    // Setup window
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Remote Desktop Controller by fit@hcmus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    setupImGui(window, gl_context);

    // Main loop
    bool done = false;
    while (!done)
    {
        handleEvents(window, done);
        updateGUI();
        renderGUI(window);
    }

    cleanup(window, gl_context);

    return 0;
}

void setupSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        exit(-1);
    }

    // From 2.0.18: Enable native IME
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
}

void setupImGui(SDL_Window *window, SDL_GLContext gl_context)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.WantCaptureKeyboard = true;
    io.WantCaptureMouse = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();
}

void eventProcessing(Computer *computer, SDL_Event &event)
{
    // KEY DOWN
    if (event.type == SDL_KEYDOWN)
    {
        computer->socket.kEvents.addKeydownEvent(convertSDLtoWIN[event.key.keysym.sym]);
    }
    // KEY UP
    else if (event.type == SDL_KEYUP)
    {
        computer->socket.kEvents.addKeyupEvent(convertSDLtoWIN[event.key.keysym.sym]);
    }
    // MOUSE DOWN
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            computer->socket.mEvents.addMouseEvent((int32_t)MOUSEEVENTF_LEFTDOWN);
        }
        else if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            computer->socket.mEvents.addMouseEvent((int32_t)MOUSEEVENTF_MIDDLEDOWN);
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            computer->socket.mEvents.addMouseEvent((int32_t)MOUSEEVENTF_RIGHTDOWN);
        }
    }
    // MOUSE UP
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            computer->socket.mEvents.addMouseEvent((int32_t)MOUSEEVENTF_LEFTUP);
        }
        else if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            computer->socket.mEvents.addMouseEvent((int32_t)MOUSEEVENTF_MIDDLEUP);
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            computer->socket.mEvents.addMouseEvent((int32_t)MOUSEEVENTF_RIGHTUP);
        }
    }
    // MOVE
    else if (event.type == SDL_MOUSEMOTION)
    {
        computer->socket.mEvents.addMouseMotion(computer->socket.mouse_pos_x, computer->socket.mouse_pos_y);
    }
    // WHEEL
    else if (event.type == SDL_MOUSEWHEEL)
    {
        computer->socket.mEvents.addMouseWheel((int32_t)event.wheel.y);
    }
}

void handleEvents(SDL_Window *window, bool &done)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
        {
            done = true;
        }
        if (event.type == SDL_QUIT)
        {
            done = true;
        }
        if (isControllingComputer == true && isThreadCreated == true)
        {
            eventProcessing(controllingCom, event);
        }
    }
}

void updateGUI()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);

    // TODO: Add GUI logic here
    mainFrame();

    ImGui::Render();
}

void renderGUI(SDL_Window *window)
{
    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr; // must have it to avoid logging windows size
    io.LogFilename = nullptr; // must have it to avoid logging windows size
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

void cleanup(SDL_Window *window, SDL_GLContext gl_context)
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
