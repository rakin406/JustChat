//
// Created by rakin on 12/20/2023.
// Entrypoint for the server.
//

#include <asio.hpp>
#include <cstdlib>
#include <iostream>
#include <list>

#include "server/server.hpp"

int main(int argc, char* argv[])
{
    using asio::ip::tcp;

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: JustChatServer <port> [<port> ...]\n";
            return 1;
        }

        asio::io_context ioContext{};
        std::list<Server> servers{};

        for (int i{ 1 }; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(ioContext, endpoint);
        }

        ioContext.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
