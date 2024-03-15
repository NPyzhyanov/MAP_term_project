#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

#include "consts.h"
#include "print_mutex.h"
#include "functions.h"

void functions::func1()
{
    std::this_thread::sleep_for(consts::perform_delay);
    
    std::lock_guard<std::mutex> lk(print_mutex);
    std::cout << "\"My name is " << __FUNCTION__ << "\" - said func1" << std::endl;
}
