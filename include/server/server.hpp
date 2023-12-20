//
// Created by rakin on 12/18/2023.
//

#ifndef JUSTCHAT_SERVER_HPP
#define JUSTCHAT_SERVER_HPP

#include <asio.hpp>

#include "room.hpp"

class Server
{
public:
    Server(asio::io_context& ioContext,
           const asio::ip::tcp::endpoint& endpoint);

private:
    asio::ip::tcp::acceptor m_acceptor;
    Room m_room{};

    void doAccept();
};

#endif //JUSTCHAT_SERVER_HPP
