# Medieval Town

This repository contains Medieval Town demo written in C++14 using OpenGL and GLSL.

## Authors
 - [Erik Matovič](https://github.com/Matovic)
 - [Peter Oliver Kolek](https://github.com/pokolek)
 - This repository is based on a course repository from [Principles of Computer Graphics and Image Processing](https://github.com/kapecp/ppgso)
 
## Data  

Used free objects and textures in the project. Compressed if needed using [ImageMagick](https://imagemagick.org/index.php):

```bash
convert image-input.jpg -define bmp:format=bmp4 -compress none image-output.bmp
```

## Installation instructions

The recommended way to build and explore the examples is to use the [CLion IDE](https://www.jetbrains.com/clion/) which is available for macOS, Linux and Windows. It is free for 30days and you can register for a [FREE LICENSE](https://www.jetbrains.com/student/) using university mail.

Depending on your system of choice you will need to install the following dependencies:
* C++14 capable compiler
* [CMake](https://cmake.org) build system - This is part of the CLion IDE installation
* [GLEW](http://glew.sourceforge.net) - The OpenGL Extension Wrangler Library for compatibility reasons
* [GLFW3](http://www.glfw.org) - Platform-independent API for creating windows, reading input, handling events, etc.
* [GLM](http://glm.g-truc.net) - C++ mathematics library for graphics software

You can also use CMake directly to generate project files for many other IDEs, see [Generic CMake instructions](#generic-instructions-for-using-cmake).

### Linux dependencies
On Linux install the following dependencies using your package manager, for example on Ubuntu open your terminal and do:

```bash
sudo apt-get install build-essential cmake libglew-dev libglfw3-dev libglm-dev
```

### Apple macOS dependencies
On macOS install [Homebrew](http://brew.sh) package manager and install the dependencies using the following commands in terminal. Note that the installation of Homebrew expects __XCode__ to be installed, this can be simply downloaded from the app store.

```bash
brew install cmake glm glfw glew libomp
```

You can also install CMake and use it to generate project files for your IDE of choice.

### Microsoft Windows dependencies
On Microsoft Windows you may try to open the `CMakeLists.txt` file with the latest Visual Studio with CMake support. The samples should work as is, binaries for the above mentioned libraries are included as part of the repository.

However the recommended way is to avoid Visual Studio and install [CLion IDE](https://www.jetbrains.com/clion/) and a __GCC__ based compiler:

* Download and install [MinGW-w64](https://sourceforge.net/projects/mingw-w64/), this is the latest GCC compiler for Windows in a nice installer.
* Make sure to install the __64bit version (select x86_64, NOT i686)__, you need to manually switch this in the installer process. 
* To avoid issues with malformed paths also edit the installation destination to C:/mingw-w64 instead of Program Files.
* Run CLion and select MinGW as the __toolchain__ when prompted (default is C:/mingw-w64/..)
* Setup the rest of the settings as you see fit.
* Open this directory and point the IDE towards the `CMakeLists.txt` file.

If CLion is too resource intensive for your system you can also try [QTCreator IDE](http://www.qt.io/ide/) that should work out of the box with MinGW and CMake.

Another alternative is to use Visual Studio 2017 with CMake support included, with this version you can open the CMakeLists.txt file directly as any other project.

## Running the examples

Regardless of the IDE of your choice you will need to manually set the __working directory__ for each sample in the IDE.

To do so in CLion click the top right button that allows you to switch targets. The selection menu will also contain _"Edit configurations"_ option that will allow you to set additional properties for each target.

Here you can browse to the [data](data) folder, you can then copy paste the path to other targets which can be switched on the left size of the window.

## Generic instructions for using CMake

Using CMake from command-line you can generate the project files as shown below. The placeholder [YOUR_GENERATOR] should be replaced with the generator appropriate for your IDE/environment. Usually removing the option entirely will generate the default for the given platform. To find out all available generators just run `cmake --help`

```bash
cd ppgso
mkdir _build && cd _build
cmake .. -G"[YOUR_GENERATOR]"
cmake --build . --target install
```

Depending on available dependencies and their installation sometimes CMake might not be able to automatically find them. You can however point CMake to the required dependencies manually by setting command-line options such as GLEW_INCLUDE_DIRS which should point to the headers of the GLEW library that you want to use. Same principle applies for other dependencies. You can alternatively use CMake GUI (cmake-gui .) and point it to the work directory, it will allow you to edit the variables more comfortably.

```bash
cd ppgso/_build
cmake .. -DGLFW_INCLUDE_DIRS="C:\libs\glfw3\include" -DGLFW_LIBRARIES="C:\libs\glfw3\glfw3.dll"
cmake --build . --target install
```

After installation the files should be installed into a new `_install` subdirectory. You can then run the examples as follows:

```bash
cd ppgso/_install
./gl_gradient
```

## Common pitfalls
#### Windows pitfalls
On Windows NEVER store projects in paths and directories that contain spaces or non-ascii characters.

If you are unfortunate enough to have a Windows username with non-ascii characters then your builds will fail unless you edit CLion configuration manually as follows:

```
# Edit idea.properties file in CLion install directory and add
idea.system.path=C:/clion/system
# Make sure the directory exists
```

#### My linux does not have OpenGL, missing -lGL
Try to install `mesa-common-dev`, `libgl1-mesa-dev` and `libglu1-mesa-dev`

## Links and Documentation
- GLM [official documentation](http://glm.g-truc.net/0.9.7/api/a00162.html) and [samples](http://glm.g-truc.net/0.9.7/code.html)
- GLFW [official documentation](http://www.glfw.org/docs/latest/) and [guide](http://www.glfw.org/docs/latest/quick.html)
- GLEW [use guide](http://glew.sourceforge.net/basic.htm)
- CMake [basic use](https://cmake.org/runningcmake/) and [tutorial](https://cmake.org/cmake-tutorial/)

