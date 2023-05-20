#pragma once

#include <common.hpp>

namespace soft_bodies::util {

std::string read_text_file(const std::filesystem::path &path);

std::vector<uint8_t> read_binary_file(const std::filesystem::path &path);

void write_file(const std::filesystem::path &path, std::string_view str);

void write_file(const std::filesystem::path &path, const std::vector<uint8_t> &buffer);

void shell(std::string cmd, const std::function<void(std::string_view)>
		&stdout_cb, bool redirect_stderr = true, size_t buffer_capacity = 1024);

}