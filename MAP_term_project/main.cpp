#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include "consts.h"
#include "functions.h"
#include "log_event.h"
#include "safe_queue.h"
#include "thread_pool.h"
#include "enqueuer.h"

#include "globals_declaration.h"

int main()
{
    const unsigned int occupied_cores = 2; // Кол-во ядер, занятых только постановкой в очередь.
    
    thread_pool pool(occupied_cores);
    
    std::thread parallel_func_calling_thread(enqueuer, &pool, available_functions::names::func2, consts::repeats_amount);
    enqueuer(&pool, available_functions::names::func1, consts::repeats_amount);

   if (parallel_func_calling_thread.joinable())
    {
        parallel_func_calling_thread.join();
    }
   
   log_event("main: done and joined");
    
   pool.stop();
   
   return 0;
}
