#ifndef LOG_H
#define LOG_H

#include <PmLog.h>

static PmLogContext getPmLogContext()
{
    static PmLogContext s_context = 0;
    if (0 == s_context)
    {
        PmLogGetContext("XPlayer", &s_context);
    }
    return s_context;
}

#endif
