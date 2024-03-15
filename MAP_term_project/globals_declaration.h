#ifndef GLOBALS_DECLARATION_H
#define GLOBALS_DECLARATION_H

std::atomic<bool> q_is_locked { false };
std::mutex print_mutex;

#endif // GLOBALS_DECLARATION_H
