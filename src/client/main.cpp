//
// Created by rakin on 12/20/2023.
// Entrypoint for the client.
//

#include <asio.hpp>
#include <iostream>
#include <thread>

#include "client/client.hpp"
#include "message.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: JustChatClient <host> <port>\n";
            return 1;
        }

        asio::io_context ioContext{};
        asio::ip::tcp::resolver resolver{ ioContext };

        auto endpoints{ resolver.resolve(argv[1], argv[2]) };
        Client client{ ioContext, endpoints };

        std::thread thread{ [&ioContext]() { ioContext.run(); } };

        char line[Message::m_MAX_BODY_LENGTH + 1]{};
        while (std::cin.getline(line, Message::m_MAX_BODY_LENGTH + 1))
        {
            Message message{};
            message.setBodyLength(std::strlen(line));
            std::memcpy(message.body(), line, message.getBodyLength());
            message.encodeHeader();
            client.write(message);
        }

        client.close();
        thread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
