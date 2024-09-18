# directories
IMGUI_DIR = lib/ImGui
# SDL2_DIR = lib/SDL2-2.28.4/x86_64-w64-mingw32/include/SDL2
PNET_DIR = lib/boost_asio_1_28_0/boost
# QT_DIR =  lib/Qt

OBJ_DIR = obj
SERVER_DIR = src
NET_DIR = $(SERVER_DIR)/Net

# compiler
CXX = g++
CXXFLAGS = -std=c++20
CXXFLAGS += -I$(PNET_DIR) -I$(PNET_DIR)/asio -I$(PNET_DIR)/asio/ts 
CXXFLAGS += -I$(NET_DIR)
CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -I$(QT_DIR)
# CXXFLAGS += -I$(SDL2_DIR)

CXXFLAGS += -g -Wall -Wformat

# output name
EXE = server

# list all .cpp file in SERVER_DIR
# SOURCES = $(SERVER_DIR)/SimpleServer.cpp
SOURCES = $(SERVER_DIR)/server.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp
# SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdlrenderer2.cpp
OBJS = $(addprefix $(OBJ_DIR)/,$(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

LIBS =

#---------------------------------------------------------------------
# BUILD FLAGS PER PLATFORM
#---------------------------------------------------------------------


ifeq ($(OS), Windows_NT)
# 	adding supported library to use ImGui
	LIBS += -lSDL2
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`
#	LIBS += -lgdi32 -lopengl32 -limm32

#	SDL
#	LIBS += -L"lib/SDL2-2.28.4/x86_64-w64-mingw32/bin"
#	LIBS += -lmingw32 -lSDL2main -lSDL2 -mwindows

#	adding ws2_32 to intializate and shutdown APIs winsocks
 	LIBS += -lpthread -lws2_32
	LIBS += -lwsock32
	LIBS += -lopencv_videoio -lopencv_highgui -lopencv_imgproc
	LIBS += -lopencv_ml -lopencv_dnn -lopencv_core -lopencv_flann -lopencv_imgcodecs

#	Qt6
#	LIBS += -L"lib/Qt" -lQt6Gui
#	LIBS += -lboost_system -lboost_thread

	CXXFLAGS += `pkg-config --cflags sdl2`

	CFLAGS = $(CXXFLAGS)
endif

#---------------------------------------------------------------------
# BUILD RULES
#---------------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SERVER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(OBJ_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	@echo Build complete for $(EXE)

clean:
	rm -f $(EXE).exe $(OBJ_DIR)/*

run:
	./$(EXE).exe