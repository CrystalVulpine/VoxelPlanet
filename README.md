# VoxelPlanet

VoxelPlanet is a sandbox made out of colored cubes. It is made using basic shaders in OpenGL 3.3. The intent is to provide an open world to create things, much in the same way you would draw a picture out of pixels, but in a 3-dimensional space. It also supports mods and has a simple mod/plugin API.

![GitHub language count](https://img.shields.io/github/languages/count/CrystalVulpine/VoxelPlanet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/CrystalVulpine/VoxelPlanet)

![](https://i.imgur.com/CsTJFOc.png)

## Controls:

* Move: WASD
* Up: Space Bar
* Down: Left Shift
* Delete cube: Left Click
* Place cube: Right Click
* Select color of the cube you're pointing at: Middle Click
* Pause/Unpause: Esc
* Hide GUI: F1
* C: Open cube color select screen
* Esc: close a screen

## Documentation

VoxelPlanet is modular and has various programmatic functions, such as a mod/plugin API and commands. Documentation and specification for those is available, and can be found hosted on [the documentation website](https://crystalvulpine.github.io/VoxelPlanet/).

## To-Do List:

- [X] Ray tracing for cube selection. Before this was implemented, the current cube was always the one the camera is inside of.

- [ ] Add an actual GUI, including a cube color picker

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

- [ ] CMake build scripts

- [ ] Smoother in-game camera and solid rendering

## Local Development:

VoxelPlanet is designed to be mostly platform independent, and should be compilable on most OSes provided that the code works with your compiler (it is designed for gcc with C++17) and that it supports the required libraries and dependencies. However, the eclipse project, if you decide to use it, may require some changes as per your specific system.

### Libraries and Dependencies:

* OpenGL
* GLEW
* GLFW
* GLM (GL Math)
