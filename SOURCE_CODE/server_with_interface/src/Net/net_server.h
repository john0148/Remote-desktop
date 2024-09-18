#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"
#include "net_image.h"

namespace net {
	template<typename T>
	class server_interface {
	public:
		// Create a server, ready to listen on specified port
		server_interface(uint16_t port)
			: m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{

		}

		virtual ~server_interface() {
			// May as well try and tidy up
			Stop();
		}

		// Starts the server!
		bool Start() {
			try {
				// Issue a task to the asio context - This is important
				// as it will prime the context with "work", and stop it
				// from exiting immediately. Since this is a server, we 
				// want it primed ready to handle clients trying to
				// connect.
				WaitForClientConnection();

				// Launch the asio context in its own thread
				m_threadContext = std::thread([this]() { m_asioContext.run(); });
			}
			catch (std::exception& e) {
				// Something prohibited the server from listening
				std::cerr << "[SERVER] Exception: " << e.what() << "\n";
				return false;
			}

			std::cout << "[SERVER] Started!\n";
			return true;
		}

		// Stops the server!
		void Stop() {
			// Request the context to close
			m_asioContext.stop();

			// Tidy up the context thread
			if (m_threadContext.joinable()) m_threadContext.join();

			// Inform someone, anybody, if they care...
			std::cout << "[SERVER] Stopped!\n";
		}

		// ASYNC - Instruct asio to wait for connection
		void WaitForClientConnection() {
			// Prime context with an instruction to wait until a socket connects. This
			// is the purpose of an "acceptor" object. It will provide a unique socket
			// for each incoming connection attempt
			m_asioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket) {
					// Triggered by incoming connection request
					if (!ec) {
						// Display some useful(?) information
						std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

						// Create a new connection to handle this client 
						std::shared_ptr<connection<T>> newconn = 
							std::make_shared<connection<T>>(connection<T>::owner::server, 
								m_asioContext, std::move(socket), m_qMessagesIn);
						
						

						// Give the user server a chance to deny connection
						if (OnClientConnect(newconn)) {								
							// Connection allowed, so add to container of new connections
							m_connection = std::move(newconn);

							// And very important! Issue a task to the connection's
							// asio context to sit and wait for bytes to arrive!
							m_connection->ConnectToClient();

							std::cout << "[" << m_connection->GetIP() << "] Connection Approved\n";
						}
						else {
							std::cout << "[-----] Connection Denied\n";

							// Connection will go out of scope with no pending tasks, so will
							// get destroyed automagically due to the wonder of smart pointers
						}
					}
					else {
						// Error has occurred during acceptance
						std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
					}

					// Prime the asio context with more work - again simply wait for
					// another connection...
					WaitForClientConnection();
				});
		}

		// Send a message to a specific client
		void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg) {
			// Check client is legitimate...
			if (client && client->IsConnected()) {
				// ...and post the message via the connection
				client->Send(msg);
			}
			else {
				// If we cant communicate with client then we may as 
				// well remove the client - let the server know, it may
				// be tracking it somehow
				OnClientDisconnect(client);

				// Off you go now, bye bye!
				client.reset();					
			}
		}
		
		// Send message to all clients
		void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr) {
			// Iterate through all clients in container
			// Check client is connected...
			if (m_connection && m_connection->IsConnected())
			{
				// ..it is!
				if (m_connection != pIgnoreClient)
					m_connection->Send(msg);
			}
			else {
				// The client couldnt be contacted, so assume it has
				// disconnected.
				OnClientDisconnect(m_connection);
				m_connection.reset();
			}

		}

		// Force server to respond to incoming messages
		void Update(size_t nMaxMessages = -1, bool bWait = false) {
			if (bWait) m_qMessagesIn.wait();

			// Process as many messages as you can up to the value
			// specified
			size_t nMessageCount = 0;
			while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty()) {
				// Grab the front message
				auto msg = m_qMessagesIn.pop_front();

				// Pass to message handler
				OnMessage(msg.remote, msg.msg);
				// JoinThread();
				nMessageCount++;
			}
		}

	protected:
		// This server class should override thse functions to implement
		// customised functionality

		// Called when a client connects, you can veto the connection by returning false
		virtual bool OnClientConnect(std::shared_ptr<connection<T>> client) {
			return false;
		}

		// Called when a client appears to have disconnected
		virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client) {

		}

		// Called when a message arrives
		virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg) {

		}


	protected:
		// Thread Safe Queue for incoming message packets
		tsqueue<owned_message<T>> m_qMessagesIn;


		// Order of declaration is important - it is also the order of initialisation
		asio::io_context m_asioContext;
		std::thread m_threadContext;

		// These things need an asio context
		asio::ip::tcp::acceptor m_asioAcceptor; // Handles new incoming connection attempts...

		// Clients will be identified in the "wider system" via an ID
		uint32_t nIDCounter = 10000;
	
	public:
		// Container of active validated connections
		std::shared_ptr<connection<T>> m_connection;
	};
}