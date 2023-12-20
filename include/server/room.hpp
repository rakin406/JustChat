//
// Created by rakin on 12/19/2023.
//

#ifndef JUSTCHAT_ROOM_HPP
#define JUSTCHAT_ROOM_HPP

#include <deque>
#include <memory>
#include <set>

#include "../message.hpp"
#include "participant.hpp"

class Room
{
public:
    static constexpr int m_MAX_RECENT_MSGS{ 100 };

    void join(std::shared_ptr<Participant> participant);
    void leave(std::shared_ptr<Participant> participant);
    void deliver(const Message& message);

private:
    std::set<std::shared_ptr<Participant>> m_participants{};
    std::deque<Message> m_recentMsgs{};
};

#endif //JUSTCHAT_ROOM_HPP
