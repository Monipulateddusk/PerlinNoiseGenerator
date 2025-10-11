# Perlin Noise Generator

This project was created during my 4th Year of University during my **Graphics Programming 2** module. I decided I wanted to create an application allowing users to utilise the algorithm first defined by Ken Perlin and create Perlin Noise images within the local directory.
I used the libary OpenGL along with the languages: GLSL and C++ to create this program allowing both 3D and 2D art to be rendered to the screen. 
Creation of a custom-built User Interface system was created making use of Lambda expressions to provide functionality. The User Interface changes the variables used within the FractalBrownianMotion method to change how the Perlin Noise texture is created.

I am extremely happy with how this project concluded. In the future I may remake the project to remove unused features and to allow users to specify a file path, but until then, enjoy!

# How to use
Clone this repository onto your local machine.
As this program makes use of OpenGL, .dll files of dependencies must be added to the Release or Debug folders inside the local directory. Download and include the following two files into the folder.
## Download Glew32.dll
glew32.dll Version 2.0.0.0 found: https://glew.sourceforge.net/
## Download SDL2.dll
SDL2.dll Version 2.26.0 found: https://github.com/libsdl-org/SDL/releases?page=6

Download the SDL-devel-2.26.0-mingw.zip and inside that folder would be the .dll inside the bin folder.


