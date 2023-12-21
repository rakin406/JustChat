//
// Created by rakin on 12/19/2023.
//

#ifndef JUSTCHAT_PARTICIPANT_HPP
#define JUSTCHAT_PARTICIPANT_HPP

#include "../message.hpp"

class Participant
{
public:
    virtual ~Participant() = default;
    virtual void deliver(const Message& message) = 0;
};

#endif //JUSTCHAT_PARTICIPANT_HPP
