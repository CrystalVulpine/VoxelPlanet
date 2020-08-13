#include "global.h"
#include <chrono>

Clock currentTimeMs() {
	return std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
}
