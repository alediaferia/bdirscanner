#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>

#ifdef DEBUG
#define err_log() \
    std::cerr << __FUNCTION__ \
        << ":" << __LINE__
#else
#define err_log() \
    std::cerr << __FUNCTION__ << " "
#endif

#endif
