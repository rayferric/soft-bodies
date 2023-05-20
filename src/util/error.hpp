#pragma once

#include <common.hpp>

namespace soft_bodies::util {

class error : public std::exception {
public:
    error(std::string message, const std::filesystem::path &file, size_t line);

	virtual ~error() = default;

#ifdef SOFT_BODIES_PLATFORM_LINUX
	const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
#else
	const char *what() const override;
#endif

	const std::filesystem::path &where() const;

	size_t at() const;

private:
	std::string message;
	std::filesystem::path file;
	size_t line;
};

}

#define SOFT_BODIES_CUSTOM_ASSERT(condition, message, custom_error)\
		if (!(condition)) {\
			throw custom_error(message, __FILE__, __LINE__);\
		}

#define SOFT_BODIES_ASSERT(condition, message)\
		SOFT_BODIES_CUSTOM_ASSERT(condition, message, ::soft_bodies::util::error)

#ifdef SOFT_BODIES_DEVELOPMENT
	#define SOFT_BODIES_DEVELOPMENT_CUSTOM_ASSERT(condition, message, custom_error)\
			SOFT_BODIES_CUSTOM_ASSERT(condition, message, custom_error)
	#define SOFT_BODIES_DEVELOPMENT_ASSERT(condition, message)\
			SOFT_BODIES_ASSERT(condition, message)
#else
	#define SOFT_BODIES_DEVELOPMENT_CUSTOM_ASSERT(condition, message, custom_error)
	#define SOFT_BODIES_DEVELOPMENT_ASSERT(condition, message)
#endif
