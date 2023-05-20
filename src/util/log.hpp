#pragma once

#include <common.hpp>

namespace soft_bodies::util::log {

void _log(const std::string &prefix, std::string message,
		const std::filesystem::path &file, size_t line);

void info(const std::string &message,
		const std::filesystem::path &file, size_t line);

void warning(const std::string &message,
		const std::filesystem::path &file, size_t line);

void error(const std::string &message,
		const std::filesystem::path &file, size_t line);

}

#define SOFT_BODIES_LOG_INFO(message)\
		::soft_bodies::util::log::info(message, __FILE__, __LINE__);
#define SOFT_BODIES_LOG_WARNING(message)\
		::soft_bodies::util::log::warning(message, __FILE__, __LINE__);
#define SOFT_BODIES_LOG_ERROR(message)\
		::soft_bodies::util::log::error(message, __FILE__, __LINE__);

#ifdef SOFT_BODIES_DEVELOPMENT
	#define SOFT_BODIES_DEVELOPMENT_LOG_INFO(message) SOFT_BODIES_LOG_INFO(message)
	#define SOFT_BODIES_DEVELOPMENT_LOG_WARNING(message) SOFT_BODIES_LOG_WARNING(message)
	#define SOFT_BODIES_DEVELOPMENT_LOG_ERROR(message) SOFT_BODIES_LOG_ERROR(message)
#else
	#define SOFT_BODIES_DEVELOPMENT_LOG_INFO(message)
	#define SOFT_BODIES_DEVELOPMENT_LOG_WARNING(message)
	#define SOFT_BODIES_DEVELOPMENT_LOG_ERROR(message)
#endif