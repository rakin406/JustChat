//
// Created by rakin on 12/21/2023.
//

#include "utils.hpp"

#include <filesystem>
#include <string>
#include <string_view>

namespace utils {

// NOTE: This code is AI generated.
std::string trim(const std::string& str, const std::string& whitespace)
{
    const auto strBegin{ str.find_first_not_of(whitespace) };
    if (strBegin == std::string::npos)
        return ""; // no content
    const auto strEnd{ str.find_last_not_of(whitespace) };
    const auto strRange{ strEnd - strBegin + 1 };
    return str.substr(strBegin, strRange);
}

// NOTE: This code is AI generated.
std::string reduce(const std::string& str, const std::string& fill,
                   const std::string& whitespace)
{
    // Trim first
    auto result{ trim(str, whitespace) };

    // Replace sub-ranges
    auto beginSpace{ result.find_first_of(whitespace) };
    while (beginSpace != std::string::npos)
    {
        const auto endSpace{ result.find_first_not_of(whitespace, beginSpace) };
        const auto range{ endSpace - beginSpace };
        result.replace(beginSpace, range, fill);
        const auto newStart{ beginSpace + fill.length() };
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

void createDirFromPath(std::string_view path)
{
    namespace fs = std::filesystem;

    fs::path parentPath{ path };
    parentPath = parentPath.parent_path();

    if (!fs::exists(parentPath))
        fs::create_directory(parentPath);
}

} // namespace utils
