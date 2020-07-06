# CPPND: Program a Concurrent Traffic Simulation

<img src="readme_files/gameplay.gif"/>

This is the capstone project for C++ Nanodegree program.
For this project the Option 1 has been chosen and has been realised as Snake-like game with bots.


## Dependencies for Running Locally
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

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Snaker`.

If you want, just copy the command below:
`mkdir build && cd build && cmake .. && make && ./Snaker`

## Project specs (from optional section)

1. Loops, Functions, I/O
    1. file src/fucntions/moveFruis.cpp constains nested functions and function moveFruitsAndPowerUps() of class Game in /src/classes/Game.cpp utilizes for loop.
    2. file operations are realised in class Settings in file /src/classes/Settings.cpp, functions loadSettings loads file and function saveSettings saves file.
    3. User input is being accepted by functions eventHandler in classes Game, GameMenu and Snake (with child classes) and is processed by functions updating setting values or calculating angle to given target (mouse pointer position inside window).