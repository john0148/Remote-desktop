#include "server.hpp"

// std::thread runServer;
// CustomServer *pServer = nullptr;

void updateServer(char port[7]){
    CustomServer server(atoi(port));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    server.Start();
    while(!server.bQuit){
        server.Update(-1, true);
    }
}

// Main code
int main(int, char **) {
    setupSDL();

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Remote Desktop Controller by fit@hcmus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, window_flags);
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

void handleEvents(SDL_Window *window, bool &done)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
        {
            // if(pServer){
                // pServer->bQuit = true;
                // if(runServer.joinable()) runServer.join();
            //     delete pServer; pServer = nullptr;
            // }
            done = true;
        }
        if (event.type == SDL_QUIT)
        {
            // if(pServer){
                // pServer->bQuit = true;
                // if(runServer.joinable()) runServer.join();
                // delete pServer; pServer = nullptr;
            // }
            done = true;
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

    static char port[7] = "12345";
    static bool isServer = false;
    static bool isThread = false;


    if(isServer == false && isThread == false){
        ImGui::Begin("Connect to server");
        ImGui::InputText("Port", port, 6);
            if (ImGui::Button("Connect")) {
                isServer = true;
            }
        ImGui::End();  
    }

    if(isServer == true && isThread == false){
        isThread = true;
        // pServer = new CustomServer(atoi(port));
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::thread(updateServer, std::ref(port)).detach();
    }


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
