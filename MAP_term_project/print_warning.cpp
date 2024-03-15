#include <iostream>
#include <string>
#include <mutex>

#include "print_mutex.h"
#include "print_warning.h"

void print_warning(std::string warning_to_print)
{
    std::lock_guard<std::mutex> lk(print_mutex);
    std::cerr << warning_to_print << std::endl;
}
