#include <util/error.hpp>

namespace soft_bodies::util {

error::error(
    std::string message, const std::filesystem::path &file, size_t line
)
    : message(std::move(message)), file(file.filename()), line(line) {}

#ifdef SOFT_BODIES_PLATFORM_LINUX
const char *error::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
#else
const char *error::what() const {
#endif
	return message.c_str();
}

const std::filesystem::path &error::where() const {
	return file;
}

size_t error::at() const {
	return line;
}

} // namespace soft_bodies::util
