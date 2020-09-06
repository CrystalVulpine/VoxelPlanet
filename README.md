# VoxelPlanet

![](https://i.imgur.com/CsTJFOc.png)

VoxelPlanet is a sandbox made out of colored cubes. It is made using basic shaders in OpenGL 3.3. The intent is to provide an open world to create things, much in the same way you would draw a picture out of pixels, but in a 3-dimensional space. It also supports mods and has a simple mod/plugin API.

[![GitHub prerelease](https://img.shields.io/github/v/release/CrystalVulpine/VoxelPlanet?include_prereleases)](https://github.com/CrystalVulpine/VoxelPlanet/releases) [![GitHub Download Count](https://img.shields.io/github/downloads/CrystalVulpine/VoxelPlanet/total)](https://github.com/CrystalVulpine/VoxelPlanet/releases) ![License](https://img.shields.io/github/license/CrystalVulpine/VoxelPlanet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/CrystalVulpine/VoxelPlanet)

![](https://i.imgur.com/fGVsUsE.png)

## Installation

VoxelPlanet does not require any installation. For Linux, just [download a release from here](https://github.com/CrystalVulpine/VoxelPlanet/releases), extract the files and run the executable. You might need to install some dependencies such as glfw3 and glew, because I'm not sure if it's legal for me to distribute them in an MIT-licensed project. For Windows and Mac, you'll need to either download the source code and build the program, or find a way to run the Linux executable. This is because I don't have a Mac, and my Windows VM has a tendency to freeze my entire computer if I run it longer than about 5 minutes. However, if anyone can provide builds for those platforms I'll be happy to release them.

## Controls:

* Move: WASD
* Up: Space Bar
* Down: Left Shift
* Delete cube: Left Click
* Place cube: Right Click
* Select color of the cube you're pointing at: Middle Click
* Pause/Unpause: Esc
* Hide GUI: F1
* Open cube color select screen: C
* Close a screen: Esc
* Ctrl+RClick: Fills cubes of one color to the selected one (like the fill tool on a paint app)
* F2: Take screenshot
* 1-0: Select cube from inventory

## Documentation

VoxelPlanet is modular and has various programmatic functions, such as a mod/plugin API and commands. Documentation and specification for those is available, and can be found hosted on [the documentation website](https://crystalvulpine.github.io/VoxelPlanet/). Additionally, there is also documentation of internal functions, classes, variables, etc. which can be found [here](https://crystalvulpine.github.io/VoxelPlanet/program).

## To-Do List:

- [x] Ray tracing for cube selection. Before this was implemented, the current cube was always the one the camera is inside of.

- [ ] Add an actual GUI

- [ ] Add settings and controls

- [ ] Improve rendering, which would boost performance and allow a larger world size and more details.

- [x] Add the ability to save worlds

- [x] Add the ability to have multiple worlds at a time

- [x] Add the ability to change your environment (like sky color, terrain generation)

- [x] Add the ability to set the size of the world as you would an image.

- [x] Create a mod api (currently only for Linux)

- [ ] Support mods on other platforms

- [ ] Sounds and music (maybe)

- [ ] Possible Android app (I've never made apps before, so I have no idea how to go about doing this, but it would be very cool)

- [x] CMake build scripts

- [ ] Smoother in-game camera and solid rendering

## Local Development:

VoxelPlanet is designed to be mostly platform independent, and should be compilable on most OSes provided that the code works with your compiler (it is designed for gcc with C++17) and that it supports the required libraries and dependencies. However, the eclipse project, if you decide to use it, may require some changes as per your specific system.

### Requirements

For Linux and macOS, you will need to install:

- A C++17 compiler

\*If you are using gcc, make sure you are using version 8 or higher, or VoxelPlanet will fail to compile.

### Libraries and Dependencies:

* OpenGL
* GLEW
* GLFW
* GLM (GL Math)
* libpng

### CMake:

If you are using CMake, you need to install these dependencies:

- [CMake](https://cmake.org/download/)
- [Conan](https://conan.io/downloads.html)

Then make sure these are all installed, along with any other libraries and dependencies:

    sudo apt install cmake clang pkg-config python3-pip
    sudo apt install xorg-dev libxcb-* libglew1.5 libglew1.5-dev libglu1-* libgl1-*
    sudo pip3 install conan

Then run build.sh. Note that you need to make sure to use a compiler that supports C++17 with <filesystem> (for gcc, that's version 8 and higher). You may also need to configure your conan profile to use the correct compiler version. For example, if you have GCC 8 installed:

    conan profile new --detect gcc8
    conan profile update settings.compiler=gcc gcc8
    conan profile update settings.compiler.version=8 gcc8
