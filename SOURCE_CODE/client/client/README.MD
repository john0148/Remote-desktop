```mermaid
classDiagram
	client <|-- mainFrame
	mainFrame <|-- TopMenuBar
	mainFrame <|-- Computer
	mainFrame <|-- Image
	TopMenuBar <|-- connectComputer
	TopMenuBar <|-- controlComputer
	TopMenuBar <|-- Computer
	TopMenuBar <|-- Image
	connectComputer <|-- Computer
	connectComputer <|-- WindowHandler
	connectComputer <|-- Image
	controlComputer <|-- Computer
	controlComputer <|-- WindowHandler
	controlComputer <|-- Image
	controlComputer <|-- imageProcessing
	
	connectComputer <|-- CONNECT_SOCKERT
	connectComputer <|-- DISCONNECT_SOCKERT
	
	class client {
		+ int main();
	}
	
	class mainFrame {
		- static std::vector~Computer~ connected_computers;
		- static std::vector~Image~ images;
		- TopMenuBar topMenuBar;
		+ void mainFrame();
	}

	class TopMenuBar {
        - void showMenuConnect(std::vector~Computer~ &connected_computers, std::vector~Image~ &images);
        - void showMenuControl(std::vector~Computer~ &connected_computers, std::vector~Image~ &images);
        
        + void showTopMenuBar(std::vector~Computer~ &connected_computers, std::vector~Image~ &images);
	}
	
	class connectComputer {
		+ void connectNewComputer(std::vector~Covmputer~ &connected_computers, std::vector~Image~ &images, bool &isOpeningConnectBox);
		+ void showConnectedComputer(std::vector~Computer~ &connected_computers, bool &isShowingConnectedComputers);
		+ void disconnectComputer(std::vector~Computer~ &connected_computers, std::vector~Image~ &images, long long unsigned int pos);
		+ void disconnectAllComputers(std::vector~Computer~ &connected_computers, std::vector~Image~ &images);
	}
	
	class controlComputer {
		+ void viewAllComputers(std::vector~Computer~ &connected_computers, std::vector~Image~ &images, bool &isViewingAllComputers);
		+ void controlComputer(Computer &computer, Image &image, bool &isControllingComputer);
	}
	
	class Computer {
        - std::string name;
        - std::string ip;
        - std::string port;
		
        + Computer();
        + Computer(std::string name, std::string ip, std::string port);
        + Computer(const Computer &computer);
        + void setName(std::string name);
        + void setIP(std::string ip);
        + void setPort(std::string port);
        + std::string getName();
        + std::string getIP();
        + std::string getPort();
    }
	
	class WindowHandler {
        - float coorX;
        - float coorY;
        - float width;
        - float height;
        
        + WindowHandler(float coorX, float coorY, float width, float height);
        + float getCoorX();
        + float getCoorY();
        + float getWidth();
        + float getHeight();
        + void setWindow();
	}
	
	class imageProcessing {
		+ bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
		+ bool LoadTextureFromData(unsigned char* image_data, GLuint* out_texture, int* out_width, int* out_height);
	}

	class Image {
			+ std::string data;
			+ int width;
			+ int height;

			+ Image();
			+ Image(std::string data, int width, int height);
			+ std::string getData();
			+ int getWidth();
			+ int getHeight();
			+ void setData(std::string data);
			+ void setWidth(int width);
			+ void setHeight(int height);
	}