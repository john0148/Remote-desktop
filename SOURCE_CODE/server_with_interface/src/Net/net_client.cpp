#include "net_client.h"

namespace net {
    template <typename T>
    client_interface<T>::client_interface() {}

    template <typename T>
    client_interface<T>::~client_interface() {
        Disconnect();
    }

    template <typename T>
    bool client_interface<T>::Connect(const std::string& host, const uint16_t port) {
        try {
            asio::ip::tcp::resolver resolver(m_context);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context, asio::ip::tcp::socket(m_context), m_qMessagesIn);

            m_connection->ConnectToServer(endpoints);

            thrContext = std::thread([this]() { m_context.run(); });
        }
        catch (std::exception& e) {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    template <typename T>
    void client_interface<T>::Disconnect() {
        if (IsConnected()) {
            m_connection->Disconnect();
        }

        m_context.stop();

        if (thrContext.joinable())
            thrContext.join();

        m_connection.release();
    }

    template <typename T>
    bool client_interface<T>::IsConnected() {
        if (m_connection)
            return m_connection->IsConnected();
        else
            return false;
    }

    template <typename T>
    void client_interface<T>::Send(const message<T>& msg) {
        if (IsConnected())
            m_connection->Send(msg);
    }

    template <typename T>
    tsqueue<owned_message<T>>& client_interface<T>::Incoming() {
        return m_qMessagesIn;
    }
}