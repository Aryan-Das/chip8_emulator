# Simple Chip8 Emulator
An emulator of a chip8 system that can run chip8 ROMS for retro games.


## Features
- **Emulated Hardware:** memory, registers, and graphics memory
- **Instruction Interpretation:** splits instruction memory into opcodes based on the program counter and executed the appropriate instruction based on the Chip8 specification
- **Graphics with SDL2** emulated graphics using black and white pixels through SDL2, using the original Chip8 fontset
- **User Input** emulated Chip8 keypad through SDL2 keyboard events
- **Command Line Interface** Execute .ch8 files at the command line

## Build and run:

Build:
```bash
g++ -std=c++20 src/*.cpp -o build/build -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2
```
Run:
```bash
build/build path-to-ch8-file.ch8
```
You can find ch8 roms from various online sources.

## File Structure
 
```
chip8_emulator/
├── build/
    ├── build          # Executable file
├── src
    ├── chip8.hpp      # Declaration for Chip8 class (Emulated CPU)
    ├── chip8.cpp      # Definition for Chip8 class (constructor, cycle(), increment_program_counter())
    ├── main.cpp       # Process user input, execute Chip8 cycles, display graphics with SDL2
├── build_instructions # Simple text file containing command to build executable, should be moved to Makefile
```

## Keymap

The original Chip8 System has the following keypad used for input:

```
+---+---+---+---+
| 1 | 2 | 3 | C |
+---+---+---+---+
| 4 | 5 | 6 | D |
+---+---+---+---+
| 7 | 8 | 9 | E |
+---+---+---+---+
| A | 0 | B | F |
+---+---+---+---+
```
The emulator maps these keys to the following keys on a QWERTY keyboard:

```
+---+---+---+---+
| 1 | 2 | 3 | 4 |
+---+---+---+---+
| Q | W | E | R |
+---+---+---+---+
| A | S | D | F |
+---+---+---+---+
| Z | X | C | V |
+---+---+---+---+
```
 
