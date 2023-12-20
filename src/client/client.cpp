//
// Created by rakin on 12/19/2023.
//

#include "client/client.hpp"

#include <asio.hpp>
#include <iostream>

#include "message.hpp"

Client::Client(
    asio::io_context& ioContext,
    const asio::ip::basic_resolver<
        asio::ip::tcp, asio::any_io_executor>::results_type& endpoints)
    : m_ioContext{ ioContext }, m_socket{ ioContext }
{
    doConnect(endpoints);
}

void Client::write(const Message& message)
{
    asio::post(m_ioContext, [this, message]() {
        bool writeInProgress{ !m_writeMsgs.empty() };
        m_writeMsgs.push_back(message);
        if (!writeInProgress)
            doWrite();
    });
}

void Client::close()
{
    asio::post(m_ioContext, [this]() { m_socket.close(); });
}

void Client::doConnect(
    const asio::ip::basic_resolver<
        asio::ip::tcp, asio::any_io_executor>::results_type& endpoints)
{
    asio::async_connect(
        m_socket, endpoints,
        [this](asio::system_error error, asio::ip::tcp::endpoint) {
        if (!error.code())
            doReadHeader();
    });
}

void Client::doReadHeader()
{
    asio::async_read(m_socket,
                     asio::buffer(m_readMsg.data(), Message::m_HEADER_LENGTH),
                     [this](asio::system_error error, std::size_t /*length*/) {
        if (!error.code() && m_readMsg.decodeHeader())
        {
            doReadBody();
        }
        else
        {
            m_socket.close();
        }
    });
}

void Client::doReadBody()
{
    asio::async_read(m_socket,
                     asio::buffer(m_readMsg.body(), m_readMsg.getBodyLength()),
                     [this](asio::system_error error, std::size_t /*length*/) {
        if (!error.code())
        {
            std::cout.write(m_readMsg.body(), m_readMsg.getBodyLength());
            std::cout << "\n";
            doReadHeader();
        }
        else
        {
            m_socket.close();
        }
    });
}

void Client::doWrite()
{
    asio::async_write(
        m_socket,
        asio::buffer(m_writeMsgs.front().data(), m_writeMsgs.front().length()),
        [this](asio::system_error error, std::size_t /*length*/) {
        if (!error.code())
        {
            m_writeMsgs.pop_front();
            if (!m_writeMsgs.empty())
                doWrite();
        }
        else
        {
            m_socket.close();
        }
    });
}
