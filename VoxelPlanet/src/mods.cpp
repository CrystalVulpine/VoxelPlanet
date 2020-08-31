#include "mods.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <cstring>

namespace fs = std::filesystem;

std::vector<void (*)()> mod_testFunc;
std::vector<void (*)()> mod_onRenderTick;
std::vector<void (*)()> mod_onWorldTick;
std::vector<void (*)()> mod_onGameStart;
std::vector<void (*)()> mod_onGameExit;
std::vector<void (*)()> mod_onWorldLoad;
std::vector<void (*)()> mod_onWorldClose;
std::vector<void (*)(Clock, Clock)> mod_onGameLoop;
std::vector<void (*)(int, char*[])> mod_processGameArgs;

void mods_testFunc() {
	for (unsigned int i = 0; i < mod_testFunc.size(); ++i) {
		mod_testFunc.at(i)();
	}
}

void mods_onRenderTick() {
	for (unsigned int i = 0; i < mod_onRenderTick.size(); ++i) {
		mod_onRenderTick.at(i)();
	}
}

void mods_onWorldTick() {
	for (unsigned int i = 0; i < mod_onWorldTick.size(); ++i) {
		mod_onWorldTick.at(i)();
	}
}

void mods_onGameStart() {
	for (unsigned int i = 0; i < mod_onGameStart.size(); ++i) {
		mod_onGameStart.at(i)();
	}
}

void mods_onGameExit() {
	for (unsigned int i = 0; i < mod_onGameExit.size(); ++i) {
		mod_onGameExit.at(i)();
	}
}

void mods_onWorldLoad() {
	for (unsigned int i = 0; i < mod_onWorldLoad.size(); ++i) {
		mod_onWorldLoad.at(i)();
	}
}

void mods_onWorldClose() {
	for (unsigned int i = 0; i < mod_onWorldClose.size(); ++i) {
		mod_onWorldClose.at(i)();
	}
}

void mods_onGameLoop(Clock loopTime, Clock lastLoopTime) {
	for (unsigned int i = 0; i < mod_onGameLoop.size(); ++i) {
		mod_onGameLoop.at(i)(loopTime, lastLoopTime);
	}
}

void mods_processGameArgs(int argc, char * argv[]) {
	for (unsigned int i = 0; i < mod_processGameArgs.size(); ++i) {
		mod_processGameArgs.at(i)(argc, argv);
	}
}


/*
 * This is the experimental code for loading mods on Windows. It is NOT YET TESTED and probably doesn't work.
 */
#if defined(_WIN32) || defined(__MINGW32__)

#include <windows.h>

void loadMods()
{
	std::string path("mods/");

    // there are no mods, so don't try to load any
    if (!fs::is_directory(path)) return;

	mod_testFunc.reserve(5);
	mod_onRenderTick.reserve(5);
	mod_onWorldTick.reserve(5);
	mod_onGameStart.reserve(5);
	mod_onGameExit.reserve(5);
	mod_onWorldLoad.reserve(5);
	mod_onWorldClose.reserve(5);
	mod_processGameArgs.reserve(5);
	mod_onGameLoop.reserve(5);

    for(auto & p: fs::recursive_directory_iterator(path))
    {
        if(strcmp(p.path().extension().c_str(), ".so") == 0) {

			HINSTANCE handle;

			if ((handle = LoadLibrary(p.path().string().c_str(), RTLD_LAZY)) != NULL) {

				const char * (*versionGetter)() = (const char * (*)())GetProcAddress(handle, "getApiVersion");

				auto error = dlerror();
				if (error != NULL) {
					std::cout << error << '\n';
					continue;
				} else {
					const char * const __restrict modApiVersion = versionGetter();
					if (strcmp(modApiVersion, API_VERSION) != 0) {
						std::cout << "Could not load " << ((const char * (*)())GetProcAddress(handle, "getModName"))() << ". Mod uses API version " << modApiVersion << " but this is " << API_VERSION << ".\n";
						continue;
					}
				}

				std::cout << "Loaded mod " << ((const char * (*)())GetProcAddress(handle, "getModName"))() << ": " << ((const char * (*)())GetProcAddress(handle, "getModDescription"))() << "\n";

				void (*testFunc)() = (void (*)())GetProcAddress(handle, "testFunc");
				if (testFunc) {
					mod_testFunc.push_back(testFunc);
				}

				void (*onRenderTick)() = (void (*)())GetProcAddress(handle, "onRenderTick");
				if (onRenderTick) {
					mod_onRenderTick.push_back(onRenderTick);
				}

				void (*onWorldTick)() = (void (*)())GetProcAddress(handle, "onWorldTick");
				if (onWorldTick) {
					mod_onWorldTick.push_back(onWorldTick);
				}

				void (*onGameStart)() = (void (*)())GetProcAddress(handle, "onGameStart");
				if (onGameStart) {
					mod_onGameStart.push_back(onGameStart);
				}

				void (*onGameExit)() = (void (*)())GetProcAddress(handle, "onGameExit");
				if (onGameExit) {
					mod_onGameExit.push_back(onGameExit);
				}

				void (*onWorldLoad)() = (void (*)())GetProcAddress(handle, "onWorldLoad");
				if (onWorldLoad) {
					mod_onWorldLoad.push_back(onWorldLoad);
				}

				void (*onWorldClose)() = (void (*)())GetProcAddress(handle, "onWorldClose");
				if (onWorldClose) {
					mod_onWorldClose.push_back(onWorldClose);
				}

				void (*onGameLoop)(Clock, Clock) = (void (*)(Clock, Clock))GetProcAddress(handle, "onGameLoop");
				if (onGameLoop) {
					mod_onGameLoop.push_back(onGameLoop);
				}

				void (*processGameArgs)(int, char * []) = (void (*)(int, char * []))GetProcAddress(handle, "processGameArgs");
				if (processGameArgs) {
					mod_processGameArgs.push_back(processGameArgs);
				}

			} else {

				std::cout << "Unable to load mod \"" << p.path().filename().string() << "\"\n";
			}
        }
    }
}


#else

#include <dlfcn.h>

void loadMods()
{
	std::string path("mods/");

    // there are no mods, so don't try to load any
    if (!fs::is_directory(path)) return;

	mod_testFunc.reserve(5);
	mod_onRenderTick.reserve(5);
	mod_onWorldTick.reserve(5);
	mod_onGameStart.reserve(5);
	mod_onGameExit.reserve(5);
	mod_onWorldLoad.reserve(5);
	mod_onWorldClose.reserve(5);
	mod_processGameArgs.reserve(5);
	mod_onGameLoop.reserve(5);

    for(auto& p: fs::recursive_directory_iterator(path))
    {
        if(strcmp(p.path().extension().c_str(), ".so") == 0) {

			void* handle;

			if ((handle = dlopen(p.path().string().c_str(), RTLD_LAZY)) != NULL) {

				const char * (*versionGetter)() = (const char * (*)())dlsym(handle, "getApiVersion");

				auto error = dlerror();
				if (error != NULL) {
					std::cout << error << '\n';
					continue;
				} else {
					const char * const __restrict modApiVersion = versionGetter();
					if (strcmp(modApiVersion, API_VERSION) != 0) {
						std::cout << "Could not load " << ((const char * (*)())dlsym(handle, "getModName"))() << ". Mod uses API version " << modApiVersion << " but this is " << API_VERSION << ".\n";
						continue;
					}
				}

				std::cout << "Loaded mod " << ((const char * (*)())dlsym(handle, "getModName"))() << ": " << ((const char * (*)())dlsym(handle, "getModDescription"))() << "\n";

				void (*testFunc)() = (void (*)())dlsym(handle, "testFunc");
				if (dlerror() == NULL) {
					mod_testFunc.push_back(testFunc);
				}

				void (*onRenderTick)() = (void (*)())dlsym(handle, "onRenderTick");
				if (dlerror() == NULL) {
					mod_onRenderTick.push_back(onRenderTick);
				}

				void (*onWorldTick)() = (void (*)())dlsym(handle, "onWorldTick");
				if (dlerror() == NULL) {
					mod_onWorldTick.push_back(onWorldTick);
				}

				void (*onGameStart)() = (void (*)())dlsym(handle, "onGameStart");
				if (dlerror() == NULL) {
					mod_onGameStart.push_back(onGameStart);
				}

				void (*onGameExit)() = (void (*)())dlsym(handle, "onGameExit");
				if (dlerror() == NULL) {
					mod_onGameExit.push_back(onGameExit);
				}

				void (*onWorldLoad)() = (void (*)())dlsym(handle, "onWorldLoad");
				if (dlerror() == NULL) {
					mod_onWorldLoad.push_back(onWorldLoad);
				}

				void (*onWorldClose)() = (void (*)())dlsym(handle, "onWorldClose");
				if (dlerror() == NULL) {
					mod_onWorldClose.push_back(onWorldClose);
				}

				void (*onGameLoop)(Clock, Clock) = (void (*)(Clock, Clock))dlsym(handle, "onGameLoop");
				if (dlerror() == NULL) {
					mod_onGameLoop.push_back(onGameLoop);
				}

				void (*processGameArgs)(int, char * []) = (void (*)(int, char * []))dlsym(handle, "processGameArgs");
				if (dlerror() == NULL) {
					mod_processGameArgs.push_back(processGameArgs);
				}

			} else {

				std::cout << "Unable to load mod \"" << p.path().filename().string() << "\"\n";
			}
        }
    }
}

#endif
