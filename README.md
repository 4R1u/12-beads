# 12 Beads

## Introduction

This is an upload of a project I did for my 1st semester of university, where we were required to implement this sort of game at the end of our Programming Fundamentals course.

### Rules

Red and green beads/counters are laid out on a board with holes/circles and lines connecting them.

Red and Green (the players) take turns moving one bead at a time.

Beads can only go from their place to an adjacent place, and may not move diagonally if their place isn't connected to any diagonal lines.

Beads can only move into an empty spot, but the one of opponent's beads can be "jumped" over and removed from the board, if it's adjacent and the spot across is empty.

When all of one player's beads are removed, the game ends and the other player wins.

## Requirements

- SDL2
- SDL_image
- SDL_ttf
- Roboto font installation files (ttf)


[This tutorial](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php) explains installing SDL2.

[This tutorial](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php) explains installing SDL\_image (and SD\_ttf).

Place the [ttf files](https://fonts.google.com/specimen/Roboto) in the data folder.

## Compilation

The previous tutorials also cover compiling the code.

I used the 64-bit binaries instead of the 32-bit ones.

The compiled executable should be run from the root folder, not the one it is stored in (if they are different)


