//
// Created by rakin on 12/19/2023.
//

#ifndef JUSTCHAT_MESSAGE_HPP
#define JUSTCHAT_MESSAGE_HPP

#include <cstdlib>

class Message
{
public:
    static constexpr std::size_t m_HEADER_LENGTH{ 4 };
    static constexpr std::size_t m_MAX_BODY_LENGTH{ 512 };

    Message();

    [[nodiscard]] const char* data() const;
    [[nodiscard]] char* data();
    [[nodiscard]] std::size_t getBodyLength() const;
    [[nodiscard]] std::size_t length() const;
    [[nodiscard]] const char* body() const;
    [[nodiscard]] char* body();
    void setBodyLength(std::size_t newLength);
    bool decodeHeader();
    void encodeHeader();

private:
    char m_data[m_HEADER_LENGTH + m_MAX_BODY_LENGTH]{};
    std::size_t m_bodyLength{};
};

#endif //JUSTCHAT_MESSAGE_HPP
