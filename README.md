# ASCIIPaint
ASCIIPaint is a small test of the asciiGL library.

# Setup
Just compile `main.cpp` with a modern C++ Compiler and run the program. AsciiGL and ATK have recently been updated to better support legacy terminals and other operating systems.


You can specify up to 4 commandline arguments:
* Foreground color (unsigned 8-bit integer)
* Background color (unsigned 8-bit integer)
* Starting X position (unsigned 64-bit integer)
* Starting Y position (unsigned 64-bit integer)

# Controls
* Use the arrow keys to move
* Use RETURN to draw
* Use DEL to delete
* Use ALT + arrow keys to move by just one cell
* Use CTRL + Up/Right to draw a line
* Use Space and backspace to go sideways
* Use F10 to clear the screen
* Use ESCAPE to close
