#pragma once

#include <common.hpp>

namespace soft_bodies::util {

template<typename T>
requires (!std::floating_point<T>)
std::string to_string(const T &value);

template<std::floating_point T>
std::string to_string(T value, bool trim_zeros = true, int32_t precision = 6);

void ltrim(std::string &str);

void rtrim(std::string &str);

void trim(std::string &str);

std::vector<std::string> split(std::string_view str,
		std::string_view delimiter, bool skip_empty = false);

void replace(std::string &str, std::string_view from, std::string_view to);

}

#include <util/string.inl>