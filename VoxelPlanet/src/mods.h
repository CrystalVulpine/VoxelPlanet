#pragma once

#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <dlfcn.h>
#include <vector>
#include <functional>
namespace fs = std::experimental::filesystem;

std::vector<void (*)()> modTestFunc;

void do_modTestFunc() {
	for (unsigned int i = 0; i < modTestFunc.size(); ++i) {
		modTestFunc.at(i)();
	}
}

#ifdef __linux__

#define API_VERSION "1.0.0"

void loadModsLinux()
{
    std::string path("mods/");

    // there are no mods, so don't try to load any
    if (!fs::is_directory(path)) {
    	return;
    }

	modTestFunc.reserve(5);
    unsigned int modCount = 0;

    for(auto& p: fs::recursive_directory_iterator(path))
    {
        if(strcmp(p.path().extension().c_str(), ".so") == 0) {

			void* handle;

			if ((handle = dlopen(p.path().string().c_str(), RTLD_LAZY)) != NULL) {

				const char* (*versionGetter)() = (const char* (*)())dlsym(handle, "getApiVersion");

				auto error = dlerror();
				if (error != NULL) {
					std::cout << error << '\n';
					continue;
				} else {
					const char* modApiVersion = versionGetter();
					if (strcmp(modApiVersion, API_VERSION) != 0) {
						std::cout << "Could not load mod " << p << ". Wrong mod API version: Mod uses " << modApiVersion << " but this is " << API_VERSION << '\n';
						continue;
					}
				}

				void (*func)() = (void (*)())dlsym(handle, "modTestFunc");

				// skip if the function isn't used by the mod
				error = dlerror();
				if (error != NULL) {
					continue;
				}

				++modCount;
				modTestFunc.push_back(func);

			} else {

				std::cout << "Unable to load mod \"" << p.path().string() << "\"\n";
			}
        }
    }
}

#define loadMods() loadModsLinux()

#else

#define loadMods()

#endif
