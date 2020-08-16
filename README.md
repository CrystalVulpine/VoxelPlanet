# VoxelPlanet

VoxelPlanet is a sandbox made out of colored cubes. It is made using basic shaders, however requires OpenGL 3.3. The intent is to provide an open world to create things, much in the same way you would draw a picture out of pixels, but in a 3-dimensional space.

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
* R: set red as the color being edited
* G: set green as the color being edited
* B: set blue as the color being edited
* Numbers from 000-255: sets the color being edited
* F1: Hide GUI

## Command Line Arguments:
* `--debug` launches VoxelPlanet in debug mode. Currently, the only difference is that in debug mode, VoxelPlanet will not capture your pointer in case the application crashes or freezes.
* `--antialiasing` plus an integer sets the multisampling/antialiasing level for the program. For example, `--antialiasing 0` would set the value to 0 (thus disabling it). The default value is 4.
* `--brightness` plus a decimal between 0.0 and 1.0 allows you to specify the brightness of the world, so you can simulate a nighttime or evening/morning effect. The default value is 1.0.
* `--skycolor` followed by 3 integers each between 0 and 255 allows you to set the color of the sky. It goes in the RGB order: the first integer specifies the amount of red color, followed by green, then blue. The default values are 128 128 255.
* `--worldsize` followed by 3 integers allows you to specify the size of the world in cubes. It is set in the order of length, width, then height. Be careful not to set it too large, as you will run out of memory if you do. This setting only applies to newly generated worlds; if an existing save file is loaded, its size will not be affected. The default size is 64x64x64.
* `--worlddir` followed by a path to a folder allows you to set the directory to load/save the world from. Paths can be both relative and absolute. The default is "world".

## Checklist:

- [X] Ray tracing for cube selection. Before this was implemented, the current cube was always the one the camera is inside of.

- [ ] Add an actual GUI

- [ ] Add settings and controls

- [ ] Improve rendering, allowing a larger world size and more details.

- [x] Add the ability to save worlds

- [x] Add the ability to have multiple worlds at a time

- [x] Add the ability to change your environment (like sky color, terrain generation)

- [ ] Implement physics (maybe)

- [x] Add the ability to set the size of the world as you would an image.

- [ ] Sounds and music

## Local Development:

VoxelPlanet is designed to be mostly platform independent, and should be compilable on most OSes provided that it works with the compiler (it is mainly built using gcc). However, the eclipse project may require some changes if you use it.

### Required Libraries:

* OpenGL
* GLEW
* GLFW
* GLM (GL Math)
* In the future, probably pthread
