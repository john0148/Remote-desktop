# libraries
LIB_DIR = lib
IMGUI_DIR = $(LIB_DIR)/ImGui
PNET_DIR = $(LIB_DIR)/boost_asio_1_28_0

# configure
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
NET_DIR = $(SRC_DIR)/Net

# compiler
CXX = g++ -std=c++20
CXXFLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -I$(PNET_DIR) -I$(PNET_DIR)/boost -I$(PNET_DIR)/boost/asio -I$(PNET_DIR)/boost/asio/ts
CXXFLAGS += -I$(NET_DIR)
CXXFLAGS += -g -Wall -Wformat

# output name
EXE = client

# list all .cpp file in SRC_DIR
SOURCES = client.cpp mainFrame.cpp ClientSocket.cpp TopMenuBar.cpp Computer.cpp WindowHandler.cpp imageProcessing.cpp Image.cpp
SOURCES += $(NET_DIR)/net_captureScreen.cpp $(NET_DIR)/net_transmit.cpp $(NET_DIR)/net_events.cpp

SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------


ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"

#	SDL
	LIBS += -lSDL2
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

	LIBS += -lpthread -lws2_32
	LIBS += -lwsock32

#	opencv	
	LIBS += -lopencv_videoio -lopencv_highgui -lopencv_imgproc
	LIBS += -lopencv_ml -lopencv_dnn -lopencv_core -lopencv_flann -lopencv_imgcodecs

	CXXFLAGS += `pkg-config --cflags sdl2`

	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(NET_DIR)/*.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(NET_DIR)/%.cpp $(NET_DIR)/*.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

build: $(BIN_DIR)/$(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

clean:
	rm -f $(BIN_DIR)/$(EXE) $(OBJS)

run:
	./$(BIN_DIR)/$(EXE).exe

br:
	make build && make run