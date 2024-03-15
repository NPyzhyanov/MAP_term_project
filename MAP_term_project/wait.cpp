#include <chrono>
#include <thread>

#include "consts.h"
#include "wait.h"

void wait()
{
    std::this_thread::sleep_for(consts::wait_delay);
}
