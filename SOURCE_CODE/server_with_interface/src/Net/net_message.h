#ifndef _NET_MESSAGE_H_
#define _NET_MESSAGE_H_
#include "net_common.h"

namespace net {
	///[OLC_HEADERIFYIER] START "MESSAGE"

	// Message Header is sent at start of all messages. The template allows us
	// to use "enum class" to ensure that the messages are valid at compile time
	template <typename T>
	struct message_header {
		T id{};
		uint32_t size = 0;
		int width = 0;
		int height = 0;
	};

	// Message Body contains a header and a std::vector, containing raw bytes
	// of infomation. This way the message can be variable length, but the size
	// in the header must be updated.
	template <typename T>
	struct message {
		// Header & Body vector
		message_header<T> header{};
		std::vector<unsigned char> body;

		// returns size of entire message packet in bytes
		size_t size() const {
			return body.size();
		}

		// Override for std::cout compatibility - produces friendly description of message
		friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		
		friend message<T> &operator<<(message<T> &msg, const std::vector<unsigned char> &data) {
			// Resize the vector and copy the data
			size_t oldSize = msg.body.size();
			msg.body.resize(oldSize + data.size());
			std::copy(data.begin(), data.end(), msg.body.begin() + oldSize);

			// Recalculate the message size
			msg.header.size = msg.size();

			return msg;
		}

		friend message<T> &operator>>(message<T> &msg, std::vector<unsigned char> &data) {
			// Move the data to the user variable and clear the original
			data = std::move(msg.body);
			msg.body.clear();

			// Recalculate the message size
			msg.header.size = msg.size();

			return msg;
		}

	};
	// An "owned" message is identical to a regular message, but it is associated with
	// a connection. On a server, the owner would be the client that sent the message, 
	// on a client the owner would be the server.

	// Forward declare the connection
	template <typename T>
	class connection;

	template <typename T>
	struct owned_message {
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		// Again, a friendly string maker
		friend std::ostream& operator<<(std::ostream& os, const owned_message<T>& msg) {
			os << msg.msg;
			return os;
		}
	};		

	///[OLC_HEADERIFYIER] END "MESSAGE"
}

#endif