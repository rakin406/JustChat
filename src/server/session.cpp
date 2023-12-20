//
// Created by rakin on 12/19/2023.
//

#include "server/session.hpp"

#include <asio.hpp>
#include <utility>

#include "message.hpp"
#include "server/room.hpp"

Session::Session(asio::ip::tcp::socket socket, Room& room)
    : m_socket{ std::move(socket) }, m_room{ room }
{
}

void Session::start()
{
    m_room.join(shared_from_this());
    doReadHeader();
}

void Session::deliver(const Message& message)
{
    bool writeInProgress{ !m_writeMsgs.empty() };
    m_writeMsgs.push_back(message);
    if (!writeInProgress)
        doWrite();
}

void Session::doReadHeader()
{
    auto self{ shared_from_this() };
    asio::async_read(
        m_socket, asio::buffer(m_readMsg.data(), Message::m_HEADER_LENGTH),
        [this, self](asio::system_error error, std::size_t /*length*/) {
        if (!error.code() && m_readMsg.decodeHeader())
        {
            doReadBody();
        }
        else
        {
            m_room.leave(shared_from_this());
        }
    });
}

void Session::doReadBody()
{
    auto self{ shared_from_this() };
    asio::async_read(
        m_socket, asio::buffer(m_readMsg.body(), m_readMsg.getBodyLength()),
        [this, self](asio::system_error error, std::size_t /*length*/) {
        if (!error.code())
        {
            m_room.deliver(m_readMsg);
            doReadHeader();
        }
        else
        {
            m_room.leave(shared_from_this());
        }
    });
}

void Session::doWrite()
{
    auto self{ shared_from_this() };
    asio::async_write(
        m_socket,
        asio::buffer(m_writeMsgs.front().data(), m_writeMsgs.front().length()),
        [this, self](asio::system_error error, std::size_t /*length*/) {
        if (!error.code())
        {
            m_writeMsgs.pop_front();
            if (!m_writeMsgs.empty())
            {
                doWrite();
            }
        }
        else
        {
            m_room.leave(shared_from_this());
        }
    });
}
