//
// Created by rakin on 12/19/2023.
//

#ifndef JUSTCHAT_CLIENT_HPP
#define JUSTCHAT_CLIENT_HPP

#include <asio.hpp>
#include <deque>

#include "../message.hpp"

class Client
{
public:
    Client(asio::io_context& ioContext,
           const asio::ip::tcp::resolver::results_type& endpoints);

    void write(const Message& message);
    void close();

private:
    asio::io_context& m_ioContext;
    asio::ip::tcp::socket m_socket;
    Message m_readMsg{};
    std::deque<Message> m_writeMsgs{};

    void doConnect(const asio::ip::tcp::resolver::results_type& endpoints);
    void doReadHeader();
    void doReadBody();
    void doWrite();
};

#endif //JUSTCHAT_CLIENT_HPP
