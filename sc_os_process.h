#ifndef SC_OS_PROCESS_H_
#define SC_OS_PROCESS_H_
#if defined(WIN64_) || defined(WIN32_)
#include <Windows.h>
#define os_memory_alloc_process()
#endif
#endif