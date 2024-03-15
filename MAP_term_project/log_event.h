#ifndef LOG_EVENT_H
#define LOG_EVENT_H

void log_event(std::string description);
void log_event(std::string description, int numerical_parameter);
void log_event(std::string description_1, std::thread::id thread_id, std::string description_2);

#endif // LOG_EVENT_H
