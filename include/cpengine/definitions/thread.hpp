#pragma once

#include <thread>
typedef std::thread::id THREAD_ID;
#define NULL_THREAD_ID THREAD_ID()

#define MINIMAL_AMOUNT_OF_CORES 4