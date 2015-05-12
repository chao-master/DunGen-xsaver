# DunGen-xsaver
*(Dungeon Generator XScreenSaver)*
Xscreensaver module That Generates Rouge-like dungeon layouts
Based off an [algorithm](http://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/) written by [munificent](https://github.com/munificent).

## Usage
Compile the program with the make file by running `make`.
Insure the program is located inside your `$PATH`.
add `programs: dungeon.app` to your .xscreensaver config.

## Configuartion
All configuration at current is done by modifiying the constants in the dungeon.h header file. Beware strange values may cause issues.
 * MAX_SIZE & MIN_SIZE controll how large rooms can be.
 * WIDTH & HEIGHT controll the size of the dungeon, actuall size is 2*SIZE+1 owning due the size of the walls.
 * ROOM_TRIES is the number of room the program tries to place
 * LOOP_FAC is the chance (repersented as 1 in LOOP_FRC) that a wall is knocked down to form an addition connection (loop) in the dungeon.

## Known Problems
* Can not handle large dungeon (segfaults)
* Some dead ends fail to be removed

## Improvments and changes that may happen
* Weight the direction corridoors go to make them strighter and less windy
* Modify the drawing to make it look more like a game.
* Maybe have a little character run through it after generation and have dead end rooms crumble after them.
* Go over spelling.
