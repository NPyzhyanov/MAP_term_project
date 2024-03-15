#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <stdexcept>
#include <string>

#include "consts.h"
#include "functions.h"
#include "q_is_empty_exception.h"
#include "log_event.h"
#include "print_warning.h"
#include "safe_queue.h"
#include "thread_pool.h"

#include "enqueuer.h"

void enqueuer(thread_pool* p_pool, available_functions::names function_to_enqueue, const int repeats_amount)
{
    for (int i = 0; i < repeats_amount; i++)
    {
        std::this_thread::sleep_for(consts::tasks_arrival_interval);
        
        switch (function_to_enqueue)
        {
        case available_functions::names::func1:
            p_pool->submit(functions::func1);
            log_event("func1 submitted", i);
            break;
        case available_functions::names::func2:
            p_pool->submit(functions::func2);
            log_event("func2 submitted", i);
            break;
        default:
            print_warning("enqueuer: unknown function");
            break;
        }
    }
}
