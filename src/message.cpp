//
// Created by rakin on 12/19/2023.
//

#include "message.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

Message::Message() : m_bodyLength{ 0 }
{
}

const char* Message::data() const
{
    return m_data;
}

char* Message::data()
{
    return m_data;
}

std::size_t Message::getBodyLength() const
{
    return m_bodyLength;
}

std::size_t Message::length() const
{
    return m_HEADER_LENGTH + m_bodyLength;
}

const char* Message::body() const
{
    return m_data + m_HEADER_LENGTH;
}

char* Message::body()
{
    return m_data + m_HEADER_LENGTH;
}

void Message::setBodyLength(std::size_t newLength)
{
    if (newLength < m_MAX_BODY_LENGTH) { m_bodyLength = newLength; }
    else if (newLength > m_MAX_BODY_LENGTH)
    {
        m_bodyLength = m_MAX_BODY_LENGTH;
    }
}

bool Message::decodeHeader()
{
    char header[m_HEADER_LENGTH + 1]{ "" };
    std::strncat(header, m_data, m_HEADER_LENGTH);
    m_bodyLength = std::atoi(header);
    if (m_bodyLength > m_MAX_BODY_LENGTH)
    {
        m_bodyLength = 0;
        return false;
    }
    return true;
}

void Message::encodeHeader()
{
    char header[m_HEADER_LENGTH + 1]{ "" };
    std::sprintf(header, "%4d", static_cast<int>(m_bodyLength));
    std::memcpy(m_data, header, m_HEADER_LENGTH);
}
