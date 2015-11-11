# Uccineering

[![Build Status](https://travis-ci.org/NigoroJr/uccineering.svg)](https://travis-ci.org/NigoroJr/uccineering)

A client for the game of Domineering.

## Disclaimer
This was a project for an artificial intelligence course, thus, the server for
this game is not publicly available. Sorry, nothing I can do about it; I
didn't write the server :(

## Description
Domineering is a game in which two players place horizontal or vertical 2-by-1
tiles on a typically 8-by-8 board and try to be the last one to make a move.
You can play the game
[here](http://www.kongregate.com/games/ginoboby/domineering).

This project has features such as:

* Recursive alpha-beta search
* transposition table

## Compiling
```sh
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## License
[WTFPL](http://www.wtfpl.net/)
