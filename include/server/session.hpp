//
// Created by rakin on 12/19/2023.
//

#ifndef JUSTCHAT_SESSION_HPP
#define JUSTCHAT_SESSION_HPP

#include <asio.hpp>
#include <deque>
#include <memory>

#include "../message.hpp"
#include "participant.hpp"
#include "room.hpp"

class Session : public Participant, public std::enable_shared_from_this<Session>
{
public:
    Session(asio::ip::tcp::socket socket, Room& room);

    void start();
    void deliver(const Message& message) override;

private:
    asio::ip::tcp::socket m_socket;
    Room& m_room;
    Message m_readMsg{};
    std::deque<Message> m_writeMsgs{};

    void doReadHeader();
    void doReadBody();
    void doWrite();
};

#endif //JUSTCHAT_SESSION_HPP
