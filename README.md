VoxelPlanet is a sandbox made out of colored cubes. It is made using basic shaders, however requires OpenGL 3.1. The intent is to provide an open world for people to create things; it's more like a 3D paint than Minecraft.

Currently, its functions are still very limited. For example, you can't set your own cube colors, and you can only place and destroy cubes at the position of your camera.

##Controls:

* Move: WASD
* Up: Space Bar
* Down: Left Shift
* Delete cube: Left Click
* Place cube: Right Click
* Select color of the selected cube: Middle Click
* Pause/Unpause: Esc

##Screenshots:

![](https://i.imgur.com/CsTJFOc.png)

##Checklist:

[ ] Ray tracing for cube selection. As of now, the current cube is always the one the camera is inside of.

[ ] Add a GUI and the ability to select the color of the cube you're placing. For now you can middle click an existing one to select its color.

[ ] Add settings and controls

[ ] Improve rendering, allowing a larger world size and more details.

[ ] Add the ability to save your world, possibly multiple

[ ] Add the ability to change your environment (like sky color, terrain generation)

[ ] Implement physics

[ ] Infinite worlds (maybe)

##Local Development:

VoxelPlanet is designed to be mostly platform independent, and should be compilable on most OSes provided that it works with the compiler (it is mainly built using gcc). However, the eclipse project may require some changes if you use it.

###Required Libraries:

* OpenGL
* GLEW
* GLFW
* GLM (GL Math)
* In the future, probably pthread
