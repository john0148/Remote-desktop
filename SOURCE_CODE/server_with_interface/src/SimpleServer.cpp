#include "olc_net.h"

class CustomServer : public net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : net::server_interface<CustomMsgTypes>(nPort) {}

	~CustomServer() {}

	void InitShareScreen() {
		InitializeScreenCapture();
	}

	void DeleteShareScreen() {
		CleanupScreenCapture();
	}

protected:
	void ServerScreen() {

		imageSendingThread = std::thread([this]()
										 {
            while (imageSending) {
				//Send message to server
				net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::ServerScreen;

				//capture screen and compress this data
				std::vector<unsigned char> data;

				CaptureScreen(data, msg.header.width, msg.header.height);

				std::vector<unsigned char> buffer;
				CompressImageTo600p(data, buffer);
				msg << buffer;

				m_connection->Send(msg);

				std::cout << data.size() << std::endl;
				std::cout << buffer.size() << std::endl;

				data.clear();
				buffer.clear();

                // Sleep to achieve 60 fps
                std::this_thread::sleep_for(std::chrono::milliseconds(160));
        	} });
	}

	virtual bool OnClientConnect(std::shared_ptr<net::connection<CustomMsgTypes>> client) {
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<net::connection<CustomMsgTypes>> client) {
		std::cout << "Removing client [" << client->GetIP() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<net::connection<CustomMsgTypes>> client, net::message<CustomMsgTypes>& msg) {
		switch (msg.header.id)	{
			// Start transmit
			case CustomMsgTypes::StopSendingImage:
				{
					std::cout << "Stop sending image successfully" << std::endl;
					// Assign sending image equal to false
					imageSending = false;
					if (imageSendingThread.joinable()) {
						imageSendingThread.join();
					}

					net::message<CustomMsgTypes> msg;
					msg.header.id = CustomMsgTypes::JoinRequest;
					client->Send(msg);
				}
				break;

			case CustomMsgTypes::SendingImageRequest:
				{
					std::cout << "Start sending images to server" << std::endl;
					imageSending = true;
					net::message<CustomMsgTypes> msg;
					msg.header.id = CustomMsgTypes::InitThreadRequest;
					client->Send(msg);
					ServerScreen();
				}
				break;

			case CustomMsgTypes::KeyEvent:
				{
					// Server has responded to a ping request
					std::vector<unsigned char> data;
					msg >> data;
					std::pair<WORD, DWORD> keyboardEvent;
					memcpy(&keyboardEvent.first, &data[0], sizeof(WORD));
					memcpy(&keyboardEvent.second, &data[sizeof(WORD)], sizeof(DWORD));
					std::cout << keyboardEvent.first << ' ' << keyboardEvent.second << '\n';

					INPUT in;
					ZeroMemory(&in, sizeof(INPUT));
					in.type = INPUT_KEYBOARD;
					in.ki.wVk = keyboardEvent.first;
					in.ki.dwFlags = keyboardEvent.second;

					SendInput(1, &in, sizeof(INPUT));
				}
				break;

			case CustomMsgTypes::MouseEvent:
				{
					// Server has responded to a ping request
					std::vector<unsigned char> data;
					msg >> data;
					mousePacket mouseEvent;
					memcpy(&mouseEvent, &data[0], sizeof(mouseEvent));
					std::cout << mouseEvent.header << ' ' << mouseEvent.val 
								<< ' ' << mouseEvent.dx << ' ' << mouseEvent.dy << '\n';

					INPUT in;
					ZeroMemory(&in, sizeof(INPUT));

					// Move
					if (mouseEvent.header == 1) {
						if (mouseEvent.dx >= 0 && mouseEvent.dx <= 1 && mouseEvent.dy >= 0 && mouseEvent.dy <= 1) {
							SetCursorPos(mouseEvent.dx * screenWidth, mouseEvent.dy * screenHeight);
						}
					}
					// Click
					else if (mouseEvent.header == 0) {
						in.type = INPUT_MOUSE;
						in.mi.dwFlags = mouseEvent.val;
					}
					// wheel
					else if (mouseEvent.header == 2) {
						in.type = INPUT_MOUSE;
						in.mi.mouseData = 150 * mouseEvent.val;
						in.mi.dwFlags = MOUSEEVENTF_WHEEL;
					}

					SendInput(1, &in, sizeof(INPUT));
				}
				break;

			case CustomMsgTypes::FinishRequest:
				{
					std::cout << "Removing client [" << client->GetIP() << "]\n";
					client.reset();
					std::cout << "Server ending" << std::endl;
					bQuit = true;
				}
				break;

			default:
				{
					// Handle unknown message type
					std::cout << "Unknown message type\n";
				}
		}
	}


public:
	bool bQuit = false;

	//Using to send ImageMessage
	bool imageSending = false;
	std::thread imageSendingThread;
};


int main(int argc, char *argv[]) {
	CustomServer server(atoi(argv[1]));
	server.InitShareScreen();
	server.Start();

	while (!server.bQuit) {
		server.Update(-1, true);
	}

	server.DeleteShareScreen();

	return 0;
}