CXX = g++
EXE1 = client
EXE2 = server
# PNET_DIR = asio-1.18.0/include
PNET_DIR = lib/boost_asio_1_28_0/boost
SERVER_DIR = Server
CLIENT_DIR = Client
OBJ_DIR = obj
NET_DIR = Net
IMGUI_DIR = lib/ImGui


SOURCES = $(SERVER_DIR)/SimpleClient.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp
# SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdlrenderer2.cpp
OBJS = $(addprefix $(OBJ_DIR)/,$(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -std=c++20
CXXFLAGS += -I$(PNET_DIR) -I$(PNET_DIR)/asio -I$(PNET_DIR)/asio/ts 
CXXFLAGS += -I$(SERVER_DIR) -I$(CLIENT_DIR) -I$(OBJ_DIR) -I$(NET_DIR)
CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(OBJ_DIR)
# CXXFLAGS += -I"C:\msys64\mingw64\bin"


CXXFLAGS += -g -Wall -Wformat
LIBS =

#---------------------------------------------------------------------
# BUILD FLAGS PER PLATFORM
#---------------------------------------------------------------------


ifeq ($(OS), Windows_NT)
# 	ECHO_MESSAGE = "MinGW"

# 	adding supported library to use ImGui
	LIBS += -lSDL2
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

#	adding ws2_32 to intializate and shutdown APIs winsocks
 	LIBS += -lpthread -lws2_32
	LIBS += -lwsock32
	LIBS += -lopencv_videoio -lopencv_highgui -lopencv_imgproc
	LIBS += -lopencv_ml -lopencv_dnn -lopencv_core -lopencv_flann -lopencv_imgcodecs
	LIBS += -L"C:\msys64\mingw64\bin" -lQt6Gui
#	LIBS += -lboost_system -lboost_thread

	CXXFLAGS += `pkg-config --cflags sdl2`
	CFLAGS = $(CXXFLAGS)
endif

#---------------------------------------------------------------------
# BUILD RULES
#---------------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SERVER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(CLIENT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(EXE1): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	@echo Build complete for $(EXE1)

$(EXE2): $(OBJ_DIR)/SimpleServer.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	@echo Build complete for $(EXE2)

clean:
	rm -f $(EXE1).exe $(EXE2).exe $(OBJ_DIR)/*
