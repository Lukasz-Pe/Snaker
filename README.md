# Snaker

<img src="readme_files/gameplay.gif"/>

This is the capstone project for C++ Nanodegree program.
For this project the Option 1 has been chosen and has been realised as Snake-like game with bots.
## Directory/project structure:
* Snaker - main directory
    * assets - here is the settings file and mapping file.
        * fonts - here are fonts used in project (Horta.ttf is used)
        * images - backgrounds, images, textures,
        * lang - translations
    * cmake - cmake search files
    * readme_files - gif which you see on the top of this page.
    * src - source files, main.h and main.cpp
        * classes - here the classes (.h and .cpp files)
            * Win - generates window, renderer and responsible for window events
            * Timer - delivers timing functions
            * Settings - load and saves settings from file
            * LTexture - loads and render textures
            * Tile - class for generating and rendering a tile for background.
            * GameMenu - generates game menu, handles events for it
            * Game - game logic, provides intermediate layer for events handling
            * Fruit - base class that generates and renders basic fruits
                * PowerUp - child class of Fruit specialised in PowerUps
            * Snake - base class for Snakes (player and bots)
                * Player - class for generating, event handling and rendering of Player's snake
                * Bot - class for generating and rendering of bot's snakes
        * functions - directory with files containing generic functions (.h and .cpp files)
            * collisions - function for detecting collisions between objects
            * initSDL - SDL function for initialising and basic configuration of library
            * moveFruit - template functions for moving fruits/powerups to the Snake
***

The projects codebase at the begineng was about 2 years old. Part of it has been rewritten, but classes like `Win`, `LTexture`, `Timer`, `Tile` were only "refurbished" to meet project's specification.

## Dependencies for compiling and running locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2
  * Linux (available through packaging system). SDL2 Packages needed:
    * SDL2 (package libsdl2 on debian):`sudo apt-get install libsdl2-dev`
    * SDL2 image (package libsdl2-image):`sudo apt-get install libsdl2-image-dev`
    * SDL2 ttf (package libsdl2-ttf):`sudo apt-get install libsdl2-ttf-dev`
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions (for linux)

1. Clone this repo.
    1. Go to directory where you will be working in.
    2. Open terminal and type `git clone https://github.com/Lukasz-Pe/Snaker.git`.
    3. Hit enter.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Snaker`.

If you want, just copy the command below:
* for build only: `mkdir build && cd build && cmake .. && make && ./Snaker`
* for clone and build: `git clone https://github.com/Lukasz-Pe/Snaker.git && cd Snaker && mkdir build && cd build && cmake .. && make && ./Snaker`

## Project specs (from optional section)

1. Loops, Functions, I/O
    1. file src/fucntions/moveFruis.cpp constains nested functions and function `moveFruitsAndPowerUps()` of class Game in /src/classes/Game.cpp utilizes for loop.
    2. file operations are realised in class Settings in file /src/classes/Settings.cpp, functions loadSettings loads file and function saveSettings saves file.
    3. User input is being accepted by functions eventHandler in classes Game, GameMenu and Snake (with child classes) and is processed by functions updating setting values or calculating angle to given target (mouse pointer position inside window).
2. Object oriented programming
    1. For classes organisation please refer to section ___Directory/project structure___.
    2. All classes use access specifiers.
    3. Classes `Snake`, `Bot`, `Player`, `Game` and `GameMenu` use initializer list.
    4. In all cases the implementation details are abstracted from interfaces.
    5. Data encapsulation is provided for all classes.
    6. Inheritance is used accordingly. Please refer to section ___Directory/project structure___.
       
       In all other cases composition was used as needed.
    7. Overloading of functions has been used in `LTexture` class.
    8. Classes `Snake`, `Player` and `Bot` are using dynamically allocated memory.
    9. Class `Snake` provide pure virtual functions which are overridden by classes `Player` and `Bot`.
    10. In class `Timer` is template function `getSeconds`. Functions in moveFruits.cpp are template functions.
    
    Class Snake has also template function called `calculateNearestTargetPosition`.
3. Memory management
    1. All classes use passing by reference and almost all functions are using pass-by-reference.
    2. No class made use of unmanaged dynamic memory allocation. Where it was possible the smart pointers were used.
    3. RAII with scopes is used in `Settings` class in functions `loadSettings` and `saveSettings` for opening, accessing, modifying and releasing the files.
    4. Classes `Player` and `Bot` implement rule of five.
    5. Move sematics is used in `Player` and `Bot` classes in implementation of rule of five.
    6. In all classes where no SDL typical were passed around, smart pointer were used.
    
    In class `Snake` is for example `shared_ptr` to `Settings` object, and in `main.cpp` is `unique_ptr` to font object.
4. Concurrency was no implemented in this project.