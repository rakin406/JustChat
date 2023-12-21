//
// Created by rakin on 12/20/2023.
// Entrypoint for the client.
//

#include <asio.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <thread>

#include "client/client.hpp"
#include "message.hpp"
#include "utils.hpp"

namespace {
constexpr std::string_view CLIENT_JSON_PATH{ PROJECT_ROOT "data/client.json" };

/**
 * @brief Checks whether username is valid.
 * @param username
 * @return true if valid.
 */
bool isUsernameValid(std::string_view username)
{
    std::string_view trimmed{ utils::reduce(username.data()) };
    return (trimmed.size() > 2 && trimmed.size() < 32);
}

/**
 * @brief Register client username.
 * @return Username
 */
std::string signUp()
{
    using namespace utils;
    using nlohmann::json;

    std::string clientName{};

    std::cout
        << "Register your username (must be between 2 and 32 characters long).\n\n";
    std::cout << "Username: ";
    std::getline(std::cin, clientName);

    // Get input again if client name doesn't meet the requirements
    while (!isUsernameValid(clientName))
    {
        std::cout
            << "Username must be between 2 and 32 characters long. Please try again.\n\n";
        std::cout << "Username: ";
        std::getline(std::cin, clientName);
    }

    // Trim whitespaces
    clientName = reduce(clientName);

    createDirFromPath(CLIENT_JSON_PATH);

    // Write to json file
    json client{};
    client["client"]["name"] = clientName;
    std::ofstream file{ CLIENT_JSON_PATH.data() };
    file << std::setw(2) << client << std::endl;

    return clientName;
}

/**
 * @brief Get client username. Register username if not found.
 * @return Username
 */
std::string login()
{
    using nlohmann::json;

    std::string clientName{};

    // Get client information if it exists
    std::ifstream file{ CLIENT_JSON_PATH.data() };
    if (file)
    {
        try
        {
            const auto clientData = json::parse(file);
            clientName = clientData["client"]["name"];
        }
        catch (const json::exception& e)
        {
            std::cerr << e.what() << "\n";
        }
    }
    else
    {
        file.close();
        clientName = signUp();
    }

    return clientName;
}

} // namespace

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: JustChatClient <host> <port>\n";
            return 1;
        }

        // TODO: Do something with this.
        std::string_view clientName{ login() };

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
