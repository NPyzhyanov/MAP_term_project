#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <stdexcept>
#include <string>

#include "print_warning.h"
#include "log_event.h"
#include "wait.h"
#include "start.h"
#include "q_is_empty_exception.h"
#include "safe_queue.h"
#include "thread_pool.h"

thread_pool::thread_pool(unsigned int occupied_cores)
{
    q_is_not_empty = false;
    stop_cmd_received = false;
    stop_working = false;
    
    unsigned int hardware_concurency = std::thread::hardware_concurrency();
    if (hardware_concurency > occupied_cores + 1)
    {
        threads_amount = hardware_concurency - occupied_cores;
    }
    else
    {
        print_warning("Too little amount of available cores. This thread pool will contain only two fictious threads. Try to allocate more cores.");
        threads_amount = 2;
    }
    
    for (unsigned int th = 0; th < threads_amount; th++)
    {
        Threads.push_back(std::thread(start<thread_pool>, this));
    }
}

thread_pool::~thread_pool()
{
    while (!stop_working)
    {
        wait();
    }
    wait();
    wait();
    
    log_event("removing all threads...");
    
    for (unsigned int th = 0; th < threads_amount; th++)
    {
        if ((Threads.at(th)).joinable())
        {
            (Threads.at(th)).join();
        }
    }
}

void thread_pool::submit(std::function<void()> func)
{
    std::lock_guard<std::mutex> submit_lk(pool_mutex);
    SQ.push(func);
    q_is_not_empty = true;
}

void thread_pool::work()
{
    log_event("thread #", std::this_thread::get_id(), " starts working");
    
    while (!stop_working)
    {
        if (!q_is_not_empty)
        {
            if (stop_cmd_received)
            {
                stop_working = true;
                break;
            }
            wait();
            continue;
        }
        
        std::function<void()> task_to_perform;
        try
        {
            std::lock_guard<std::mutex> work_lk(pool_mutex);
            task_to_perform = SQ.pop();
        }
        catch(const q_is_empty_exception& empty_ex)
        {
            q_is_not_empty = false;
            continue;
        }
        catch (const std::exception& ex)
        {
            print_warning(ex.what());
            continue;
        }
        catch(...)
        {
            print_warning("Unknown exception while trying to call safe_queue::pop() occured");
            continue;
        }
        task_to_perform();
    }
    
    log_event("thread #", std::this_thread::get_id(), " stops working");
}

void thread_pool::stop()
{
    std::lock_guard<std::mutex> stop_lk(pool_mutex);
    stop_cmd_received = true;
}
