// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#define SAFE_RELEASE(p) \
  do                    \
  {                     \
	if(p)               \
	{                   \
	  (p)->Release();   \
	  (p) = NULL;       \
	}                   \
  } while((void)0, 0)


#define LOGGING_ACTIVE 

#ifdef LOGGING_ACTIVE
#define LOG(string) logprintf(string)
#else
#define LOG(string)
#endif

#ifdef LOGGING_ACTIVE

#include <iomanip>
#include <string>
#include <fstream>

void logprintf(const std::string& logMsg);

void prepareOfs(std::string fileName);

void closeOfs();

static inline int64_t GetTicks()
{
	LARGE_INTEGER ticks;
	if (!QueryPerformanceCounter(&ticks))
		return 0;

	return ticks.QuadPart;
}

template< typename T >
static inline std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x"
		<< std::setfill('0')
		<< std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}

static inline std::string ToString(REFIID guid)
{
	char guid_string[37]; // 32 hex chars + 4 hyphens + null terminator

	snprintf(
		guid_string, sizeof(guid_string),
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return guid_string;
}

#endif


#endif //PCH_H
