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

		
		friend message<T> &operator<<(message<T> &msg, const int &data) {
			// Check that the type of the data being pushed is trivially copyable
			static_assert(std::is_standard_layout<int>::value, "Data is too complex to be pushed into vector");

			// Cache current size of vector, as this will be the point we insert the data
			size_t i = msg.body.size();

			// Resize the vector by the size of the data being pushed
			msg.body.resize(msg.body.size() + sizeof(int));

			// Physically copy the data into the newly allocated vector space
			std::memcpy(msg.body.data() + i, &data, sizeof(int));

			// Recalculate the message size
			msg.header.size = msg.size();

			// Return the target message so it can be "chained"
			return msg;
		}


		friend message<T> &operator<<(message<T> &msg, const std::vector<unsigned char> &data) {
			// Cache current size of vector, as this will be the point we insert the data
			size_t i = msg.body.size();

			// Resize the vector by the size of the data being pushed
			msg.body.resize(msg.body.size() + data.size());

			// Physically copy the data into the newly allocated vector space
			// std::memcpy(msg.body.data() + i, data.data(), sizeof(char) * data.size());
			for (int j = 0; j < (int)data.size(); ++j) msg.body[i + j] = data[j];
			//msg.body = data;

			// Recalculate the message size
			msg.header.size = msg.size();

			// Return the target message so it can be "chained"
			return msg;
		}

		friend message<T> &operator>>(message<T> &msg, std::vector<unsigned char> &data) {
			// Check that the type of the data being pushed is trivially copyable
			// static_assert(std::is_standard_layout<std::vector<char>>::value, "Data is too complex to be pulled from vector");

			// Physically copy the data from the vector into the user variable
			data.resize(msg.body.size());
			for (int i = 0; i < (int)msg.body.size(); ++i) data[i] = msg.body[i];

			// Shrink the vector to remove read bytes, and reset end position
			msg.body.clear();

			// Recalculate the message size
			msg.header.size = msg.size();

			// Return the target message so it can be "chained"
			return msg;
		}

		friend message<T>& operator >> (message<T>& msg, int& data) {
			// Check that the type of the data being pushed is trivially copyable
			static_assert(std::is_standard_layout<int>::value, "Data is too complex to be pulled from vector");

			// Cache the location towards the end of the vector where the pulled data starts
			size_t i = msg.body.size() - sizeof(int);

			// Physically copy the data from the vector into the user variable
			std::memcpy(&data, msg.body.data() + i, sizeof(int));

			// Shrink the vector to remove read bytes, and reset end position
			msg.body.resize(i);

			// Recalculate the message size
			msg.header.size = msg.size();

			// Return the target message so it can be "chained"
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