#pragma once

#if defined(_WIN32)
#include "WindowsProcess.hpp"
#define _PROCESS 1
#elif defined (__unix__)
#include "UnixProcess.hpp"
#define _PROCESS 1
#else
#define _PROCESS 0
#endif


