# Langton's Ant Simulation

This is a Windows-based implementation of Langton's Ant, a cellular automaton that creates fascinating geometric patterns through simple rules.

## Requirements

- Windows operating system
- MinGW-w64 (for compilation)
- GDI32 library (included with Windows)

## Quick Start

1. Download and install MinGW-w64 from [here](https://www.mingw-w64.org/downloads/) if you don't have it already
2. Add MinGW-w64's bin directory to your system's PATH environment variable
3. Double-click `run.bat` to compile and run the program

## Manual Compilation

If you prefer to compile manually, open a command prompt and run:

```bash
g++ -o langton_ant.exe langton_ant.cpp -lgdi32
```

Then run the program:

```bash
.\langton_ant.exe
```

## Controls

- **Up Arrow**: Increase simulation speed
- **Down Arrow**: Decrease simulation speed
- **Escape**: Exit the program

## How It Works

Langton's Ant follows these simple rules:
1. At a white square, turn 90° right, flip the color of the square, and move forward one unit
2. At a black square, turn 90° left, flip the color of the square, and move forward one unit

The ant's path creates complex geometric patterns over time, demonstrating how simple rules can create emergent complexity. #   L a n g t o n _ A n t  
 