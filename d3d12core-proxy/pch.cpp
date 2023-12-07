// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#ifdef LOGGING_ACTIVE

std::ofstream ofs;

void logprintf(const std::string& logMsg)
{
	ofs << GetTicks() << ": " << logMsg << '\n';
	ofs.flush();
}

void prepareOfs(std::string fileName)
{
	ofs = std::ofstream(fileName, std::ios_base::out | std::ios_base::app);
}

void closeOfs()
{
	ofs.close();
}


#endif

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
