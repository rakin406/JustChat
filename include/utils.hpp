//
// Created by rakin on 12/21/2023.
// Contains useful utilities to aid development.
//

#ifndef JUSTCHAT_UTILS_HPP
#define JUSTCHAT_UTILS_HPP

#include <string>

namespace utils {
/**
 * @brief Trims leading and trailing whitespace from string.
 * @param str String
 * @param whitespace
 * @return String
 */
std::string trim(const std::string& str, const std::string& whitespace = " \t");

/**
 * @brief Trims leading, trailing, and excessive internal whitespace from string.
 * @param str String
 * @param fill
 * @param whitespace
 * @return String
 */
std::string reduce(const std::string& str, const std::string& fill = " ",
                   const std::string& whitespace = " \t");
} // namespace utils

#endif //JUSTCHAT_UTILS_HPP
