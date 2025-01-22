# Conway's Game of Life

Conway's game of life implementation in pure C using SDL

## Purpose

The main purpose is to teach myself about SDL to use it later on bigger project. \
This project also allow me to improve in C which is language I liked but still have so much to learn about.

## Requirement

- [SDL](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.11)
- C compiler
- CMake

## Usage

### Using CMake

```bash
mkdir build
./build.sh
./build/gol
```

### Using gcc

```bash
gcc -o gol src/main.c `sdl2-config --cflags --libs`
./gol
```

## How to use

There is multiple features you can use to upgrade your experience while using my Game of Life implementation \

List of the usable keybindings : 

- SPACE = pause / unpause the simulation 
- "r" = reset the grid (all the cells are dead), usable only in paused context 
- "g" = regenerate the grid (create random deads and alives cells), usable only in paused context 
- "+" = zoom in 
- "-" = zomm out 

## Author 

This project is entirely made by me (ASTOLFI Vincent). I suggest you to check on my github profile if you want to see the other project I've done for my studies or the ones I do in my free time.
