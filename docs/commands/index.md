# Commands

Here you will find documentation for VoxelPlanet's various commands.

## Command Line Arguments

When starting VoxelPlanet, you can specify the following command line arguments:

* `--debug` launches VoxelPlanet in debug mode. Currently, the only difference is that in debug mode, VoxelPlanet will not capture your pointer in case the application crashes or freezes.
* `--antialiasing` plus an integer sets the multisampling/antialiasing level for the program. For example, `--antialiasing 0` would set the value to 0 (thus disabling it). The default value is 4.
* `--brightness` plus a decimal between 0.0 and 1.0 allows you to specify the brightness of the world, so you can simulate a nighttime or evening/morning effect. The default value is 1.0.
* `--skycolor` followed by 3 integers each between 0 and 255 allows you to set the color of the sky. It goes in the RGB order: the first integer specifies the amount of red color, followed by green, then blue. The default values are 128 128 255.
* `--worldsize` followed by 3 integers allows you to specify the size of the world in cubes. It is set in the order of length, width, then height. Be careful not to set it too large, as you will run out of memory if you do. This setting only applies to newly generated worlds; if an existing save file is loaded, its size will not be affected. The default size is 64x64x64.
* `--worlddir` followed by a path to a folder allows you to set the directory to load/save the world from. Paths can be both relative and absolute. The default is "world".
