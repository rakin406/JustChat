//
// Created by rakin on 12/18/2023.
// Chat server
//

#include "server/server.hpp"

#include <asio.hpp>
#include <memory>
#include <utility>

#include "server/session.hpp"

Server::Server(asio::io_context& ioContext,
               const asio::ip::tcp::endpoint& endpoint)
    : m_acceptor{ ioContext, endpoint }
{
    doAccept();
}

void Server::doAccept()
{
    m_acceptor.async_accept(
        [this](asio::system_error error, asio::ip::tcp::socket socket) {
        if (!error.code())
        {
            std::make_shared<Session>(std::move(socket), m_room)->start();
        }

        doAccept();
    });
}
