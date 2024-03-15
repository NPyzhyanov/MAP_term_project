#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>
#include <stdexcept>
#include <string>

#include "q_is_empty_exception.h"
#include "log_event.h"
#include "print_warning.h"
#include "wait.h"

extern std::atomic<bool> q_is_locked;

template <class Function>
class safe_queue
{
public:
    safe_queue() = default;
    
    void push(Function task_to_enqueue)
    {
        this->lock();
        Q.push(task_to_enqueue);
        this->unlock();
    };
    
    Function pop()
    {
        Function task_to_perform;
        this->lock();
        if (!Q.empty())
        {
            task_to_perform = Q.front();
            Q.pop();
        }
        else
        {
            this->unlock();
            throw q_is_empty_exception("The queue is empty");
        }
        this->unlock();
        return task_to_perform;
    };
    
    std::mutex queue_mutex;
    
private:
    std::queue<Function> Q;
    
    void lock()
    {
        std::chrono::milliseconds delay {5};
        int protect_cntr = 0;
        bool try_again = false;
        while(q_is_locked)
        {
            std::this_thread::sleep_for(delay);
            protect_cntr++;
            if (protect_cntr >= 200)
            {
                print_warning("void safe_queue::lock(): Too long while-loop processing...");
                if (!try_again)
                {
                    try_again = true;
                    wait();
                    log_event("void safe_queue::lock(): Trying to lock the queue mutex one more time...");
                    continue;
                }
                else
                {
                    print_warning("void safe_queue::lock(): The queue mutex cannot be locked.");
                    return;
                }
            }
        }
        
        q_is_locked = true;
        queue_mutex.lock();
    };
    
    void unlock()
    {
        if (!q_is_locked)
        {
            print_warning("void safe_queue::unlock(): variable 'q_is_locked' has already been initialized as 'false'");
            return;
        }
        queue_mutex.unlock();
        q_is_locked = false;
    };

};

#endif // SAFE_QUEUE_H
