#ifndef THREAD_POOL_H
#define THREAD_POOL_H

class thread_pool
{
public:
    thread_pool(unsigned int occudied_cores);
    ~thread_pool();

    thread_pool(const thread_pool& other) = delete;
    thread_pool& operator=(const thread_pool& other) = delete;

    thread_pool(thread_pool&& other) = delete;
    thread_pool& operator=(thread_pool&& other) = delete;
    
    void submit(std::function<void()> task_to_enqueue);
    void work();
    void stop();
    
    std::mutex pool_mutex;

private:
    std::vector<std::thread> Threads;
    safe_queue<std::function<void()>> SQ;
    
    unsigned int threads_amount;
    
    std::atomic<bool> q_is_not_empty;
    std::atomic<bool> stop_cmd_received;
    std::atomic<bool> stop_working;

};

#endif // THREAD_POOL_H
