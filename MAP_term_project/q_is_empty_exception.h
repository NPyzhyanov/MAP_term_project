#ifndef Q_IS_EMPTY_EXCEPTION_H
#define Q_IS_EMPTY_EXCEPTION_H

class q_is_empty_exception : public std::exception
{
public:
    q_is_empty_exception(const std::string what_arg_) : what_arg(what_arg_) {}
    
    const char* what() const noexcept { return what_arg.c_str(); }
    
private:
    std::string what_arg;
};

#endif // Q_IS_EMPTY_EXCEPTION_H
