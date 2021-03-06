#pragma once

#include "slience/base/config.hpp"

#ifdef M_PLATFORM_WIN
// for windows
#include <stdio.h>
#include <functional>
#include <unordered_map>

#if defined(_MSC_VER) && _MSC_VER<1900   // MS VC++ 14.0 _MSC_VER = 1900 (Visual Studio 2015)
#define snprintf sprintf_s  
#endif 

#define myfopen(path, mode, flag) _fsopen(path, mode, flag)
#define myfscanf fscanf_s

#ifndef m_function_t
#define m_function_t std::function
#endif

#ifndef m_bind_t
#define m_bind_t std::bind
#endif

#ifndef m_unorder_map_t
#define m_unorder_map_t std::unordered_map
#endif

#else
// for linux
#include <unistd.h>
#include <tr1/functional>
#include <tr1/unordered_map>

// windows's macro, just for compatibility
#define _SH_DENYRW      0x10    // deny read/write mode
#define _SH_DENYWR      0x20    // deny write mode
#define _SH_DENYRD      0x30    // deny read mode
#define _SH_DENYNO      0x40    // deny none mode
#define _SH_SECURE      0x80    // secure mode

#define Sleep(milsecond) usleep(milsecond * 1000)
#define GetCurrentProcessId getpid
#define myfopen(path, mode, flag) fopen(path, mode)
#define myfscanf fscanf

#ifndef m_function_t
#define m_function_t std::tr1::function
#endif

#ifndef m_bind_t
#define m_bind_t std::tr1::bind
#endif

#ifndef m_unorder_map_t
#define m_unorder_map_t std::tr1::unordered_map
#endif

#endif
