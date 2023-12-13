#include "Jauntlet.h"
#include "Errors.h"
#include "Filesystems/FileManager.h"
#include <fstream>

#if _WIN32
#include <Windows.h>
#endif

namespace Jauntlet {
	
	int init() {
		#if _WIN32 && NDEBUG
		// Hide the windows terminal. We hate the windows terminal.
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		#endif

		// We only want to use our termination script on release builds so that we don't intercept crash logging for IDEs,
		// this is actually only useful for Linux developers, where all builds will run the termination script whilst developing,
		// Windows users will find that when running through VS that VS overrides std::set_terminate anyways. -xm
		#ifdef NDEBUG 
		FileManager::createFolder("Logs");
		// open the file, therefore clearing it.
		std::ofstream errorFile;
		errorFile.open("Logs/Latest.log");
		errorFile.close();

		std::set_terminate(Jauntlet::terminate);
		#endif

		// always dump the logs at exit.
		std::atexit(Jauntlet::dumpLog);
		return 0;
	}
}