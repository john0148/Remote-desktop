#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"


namespace net {
    template<typename T>
    class connection : public std::enable_shared_from_this<connection<T>> {
    public:
        enum class owner {
            server,
            client
        };

    public:
        connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn);
        virtual ~connection();

        std::string GetIP() const;

        void ConnectToClient();
        void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);
        void Disconnect();
        bool IsConnected() const;
        void StartListening();

        void Send(const message<T>& msg);

    private:
        void WriteHeader();
        void WriteBody();
        void ReadHeader();
        void ReadBody();
        void AddToIncomingMessageQueue();

    protected:
        asio::io_context& m_asioContext;
        asio::ip::tcp::socket m_socket;
        tsqueue<owned_message<T>>& m_qMessagesIn;
        tsqueue<message<T>> m_qMessagesOut;
        message<T> m_msgTemporaryIn;
        owner m_nOwnerType = owner::server;
    };
}

#include "net_connection.cpp"