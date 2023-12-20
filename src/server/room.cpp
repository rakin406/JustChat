//
// Created by rakin on 12/19/2023.
//

#include "server/room.hpp"

#include <memory>

#include "message.hpp"
#include "server/participant.hpp"

void Room::join(std::shared_ptr<Participant> participant)
{
    m_participants.insert(participant);
    for (auto msg : m_recentMsgs)
        participant->deliver(msg);
}

void Room::leave(std::shared_ptr<Participant> participant)
{
    m_participants.erase(participant);
}

void Room::deliver(const Message& message)
{
    m_recentMsgs.push_back(message);
    if (m_recentMsgs.size() > m_MAX_RECENT_MSGS)
        m_recentMsgs.pop_front();
    for (auto participant : m_participants)
        participant->deliver(message);
}
