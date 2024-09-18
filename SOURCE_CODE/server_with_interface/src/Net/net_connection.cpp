#include "net_connection.h"

namespace net {
    template<typename T>
    connection<T>::connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn)
        : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn) {
        m_nOwnerType = parent;
    }

    template<typename T>
    connection<T>::~connection() {}

    template<typename T>
    std::string connection<T>::GetIP() const {
        try {
            return m_socket.remote_endpoint().address().to_string();
        }
        catch (const std::exception &e) {
            std::cerr << "Error getting remote IP: " << e.what() << std::endl;
            return "";
        }
    }

    template<typename T>
    void connection<T>::ConnectToClient() {
        if (m_nOwnerType == owner::server) {
            if (m_socket.is_open()) {
                ReadHeader();
            }
        }
    }

    template<typename T>
    void connection<T>::ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
        if (m_nOwnerType == owner::client) {
            asio::async_connect(m_socket, endpoints, [this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
                if (!ec) {
                    ReadHeader();
                }
            });
        }
    }

    template<typename T>
    void connection<T>::Disconnect() {
        if (IsConnected())
            asio::post(m_asioContext, [this]() { m_socket.close(); });
    }

    template<typename T>
    bool connection<T>::IsConnected() const {
        return m_socket.is_open();
    }

    template<typename T>
    void connection<T>::StartListening() {}

    template<typename T>
    void connection<T>::Send(const message<T>& msg) {
        asio::post(m_asioContext, [this, msg]() {
            bool bWritingMessage = !m_qMessagesOut.empty();
            m_qMessagesOut.push_back(msg);
            if (!bWritingMessage) {
                WriteHeader();
            }
        });
    }

    template<typename T>
    void connection<T>::WriteHeader() {
        asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    if (m_qMessagesOut.front().body.size() > 0) {
                        WriteBody();
                    }
                    else {
                        m_qMessagesOut.pop_front();
                        if (!m_qMessagesOut.empty()) {
                            WriteHeader();
                        }
                    }
                }
                else {
                    std::cout << "Write Header Fail.\n";
                    m_socket.close();
                }
            });
    }

    template<typename T>
    void connection<T>::WriteBody() {
        asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    m_qMessagesOut.pop_front();
                    if (!m_qMessagesOut.empty()) {
                        WriteHeader();
                    }
                }
                else {
                    std::cout << "Write Body Fail.\n";
                    m_socket.close();
                }
            });
    }

    template<typename T>
    void connection<T>::ReadHeader() {
        asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    if (m_msgTemporaryIn.header.size > 0) {
                        m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
                        ReadBody();
                    }
                    else {
                        AddToIncomingMessageQueue();
                    }
                }
                else {
                    std::cout << "Read Header Fail.\n";
                    m_socket.close();
                }
            });
    }

    template<typename T>
    void connection<T>::ReadBody() {
        asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    AddToIncomingMessageQueue();
                }
                else {
                    std::cout << "Read Body Fail.\n";
                    m_socket.close();
                }
            });
    }

    template<typename T>
    void connection<T>::AddToIncomingMessageQueue() {
        if (m_nOwnerType == owner::server)
            m_qMessagesIn.push_back({ this->shared_from_this(), m_msgTemporaryIn });
        else
            m_qMessagesIn.push_back({ nullptr, m_msgTemporaryIn });

        ReadHeader();
    }
}