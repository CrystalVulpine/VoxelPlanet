#include "mods.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <cstring>

#if defined(_WIN32) || defined(__MINGW32__)

#include <windows.h>
#define dllExt ".dll"
static inline void * loadMod(const char * const __restrict path) { return LoadLibrary(path); }
static inline void * loadModFunc(void * __restrict handle, const char * const __restrict name) { return GetProcAddress(handle, name); }

#else

#include <dlfcn.h>
#define dllExt ".so"
static inline void * loadMod(const char * const __restrict path) { return dlopen(path, RTLD_LAZY); }
static inline void * loadModFunc(void * __restrict handle, const char * const __restrict name) { return dlsym(handle, name); }

#endif

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
        if(strcmp(p.path().extension().c_str(), dllExt) == 0) {

			void * __restrict handle;

			if ((handle = loadMod(p.path().string().c_str())) != NULL) {

				const char * (*versionGetter)() = (const char * (*)())loadModFunc(handle, "getApiVersion");

				if (!versionGetter) {
					std::cout << "No mod API version found for mod "  << ((const char * (*)())loadModFunc(handle, "getModName"))() << ", cannot load.\n";
					continue;
				} else {
					const char * const __restrict modApiVersion = versionGetter();
					if (strcmp(modApiVersion, API_VERSION) != 0) {
						std::cout << "Could not load " << ((const char * (*)())loadModFunc(handle, "getModName"))() << ". Mod uses API version " << modApiVersion << " but this is " << API_VERSION << ".\n";
						continue;
					}
				}

				std::cout << "Loaded mod " << ((const char * (*)())loadModFunc(handle, "getModName"))() << ": " << ((const char * (*)())loadModFunc(handle, "getModDescription"))() << "\n";

				void (*testFunc)() = (void (*)())loadModFunc(handle, "testFunc");
				if (testFunc) {
					mod_testFunc.push_back(testFunc);
				}

				void (*onRenderTick)() = (void (*)())loadModFunc(handle, "onRenderTick");
				if (onRenderTick) {
					mod_onRenderTick.push_back(onRenderTick);
				}

				void (*onWorldTick)() = (void (*)())loadModFunc(handle, "onWorldTick");
				if (onWorldTick) {
					mod_onWorldTick.push_back(onWorldTick);
				}

				void (*onGameStart)() = (void (*)())loadModFunc(handle, "onGameStart");
				if (onGameStart) {
					mod_onGameStart.push_back(onGameStart);
				}

				void (*onGameExit)() = (void (*)())loadModFunc(handle, "onGameExit");
				if (onGameExit) {
					mod_onGameExit.push_back(onGameExit);
				}

				void (*onWorldLoad)() = (void (*)())loadModFunc(handle, "onWorldLoad");
				if (onWorldLoad) {
					mod_onWorldLoad.push_back(onWorldLoad);
				}

				void (*onWorldClose)() = (void (*)())loadModFunc(handle, "onWorldClose");
				if (onWorldClose) {
					mod_onWorldClose.push_back(onWorldClose);
				}

				void (*onGameLoop)(Clock, Clock) = (void (*)(Clock, Clock))loadModFunc(handle, "onGameLoop");
				if (onGameLoop) {
					mod_onGameLoop.push_back(onGameLoop);
				}

				void (*processGameArgs)(int, char * []) = (void (*)(int, char * []))loadModFunc(handle, "processGameArgs");
				if (processGameArgs) {
					mod_processGameArgs.push_back(processGameArgs);
				}

			} else {

				std::cout << "Unable to load mod \"" << p.path().filename().string() << "\"\n";
			}
        }
    }
}
