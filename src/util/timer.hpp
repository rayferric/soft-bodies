#pragma once

#include <common.hpp>

namespace soft_bodies::util {

class timer {
public:
	timer();

	bool running() const;

	void start();

	void stop();

	void reset();

	double elapsed() const;

private:
	bool _running;
	std::chrono::steady_clock::time_point start_time, end_time;
};

}
