VoxelPlanet is a sandbox made out of colored cubes. It is made using basic shaders, however requires OpenGL 3.3. The intent is to provide an open world for people to create things; it's more like a 3D paint than Minecraft.

Currently, its functions are still very limited.

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

## Screenshots:

![](https://i.imgur.com/CsTJFOc.png)

## Checklist:

- [X] Ray tracing for cube selection. Before this was implemented, the current cube was always the one the camera is inside of.

- [x] Add a GUI and the ability to select the color of the cube you're placing. (Only partially finished).

- [ ] Add settings and controls

- [ ] Improve rendering, allowing a larger world size and more details.

- [x] Add the ability to save your world, possibly multiple

- [ ] Add the ability to change your environment (like sky color, terrain generation)

- [ ] Implement physics (maybe)

- [ ] Infinite worlds (maybe), or the ability to set the size of the world as you would an image.

- [ ] Sounds and music

## Local Development:

VoxelPlanet is designed to be mostly platform independent, and should be compilable on most OSes provided that it works with the compiler (it is mainly built using gcc). However, the eclipse project may require some changes if you use it.

### Required Libraries:

* OpenGL
* GLEW
* GLFW
* GLM (GL Math)
* In the future, probably pthread
