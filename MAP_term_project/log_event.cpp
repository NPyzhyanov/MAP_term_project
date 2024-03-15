#include <iostream>
#include <string>
#include <mutex>
#include <thread>

#include "print_mutex.h"
#include "log_event.h"

void log_event(std::string description)
{
    std::lock_guard<std::mutex> lk(print_mutex);
    std::cout << description << std::endl;
}

void log_event(std::string description, int numerical_parameter)
{
    std::lock_guard<std::mutex> lk(print_mutex);
    std::cout << description << " (" << numerical_parameter << ")" << std::endl;
}

void log_event(std::string description_1, std::thread::id thread_id, std::string description_2)
{
    std::lock_guard<std::mutex> lk(print_mutex);
    std::cout << description_1 << thread_id << description_2 << std::endl;
}
