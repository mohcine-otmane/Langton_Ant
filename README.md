# Langton's Ant Simulation

A visually enhanced implementation of Langton's Ant cellular automaton with multiple ants, vibrant colors, and interactive controls.

![Langton's Ant Simulation](screenshot.png)

## Overview

This project implements Langton's Ant, a two-dimensional cellular automaton that creates complex patterns from simple rules. The simulation features multiple ants with different colors, vibrant visual effects, and an interactive interface for controlling the simulation in real-time.

## Features

- **Multiple Ants**: The simulation supports up to 100 ants, each with its own unique color and behavior.
- **Vibrant Visual Effects**: 
  - Enhanced color palette with 12 distinct colors
  - Color blending between adjacent cells
  - Glow effects for ants
  - Vibrant color variations
- **Interactive Controls**:
  - Pan the view using arrow keys
  - Adjust simulation speed with W/S keys
  - Change ant size with A/D keys
  - Reset the simulation with R key
- **Infinite Canvas**: Pan around the simulation to explore patterns
- **Complex Ant Behavior**: Ants follow enhanced movement patterns based on cell colors and position

## Technical Details

### Grid System
- Grid size: 100x100 cells
- Cell size: Variable (default is window size divided by grid size minus 2)
- Background color: Dark gray (RGB: 20, 20, 30)

### Color System
- 12 base colors for cells
- 10 unique colors for ants
- Color blending algorithm with weighted averaging
- Vibrant color variations with boosted saturation

### Ant Behavior
- Each ant follows complex movement patterns
- Ants turn based on cell color and position
- Ants cannot wrap around the grid (they turn around at boundaries)
- Ants occasionally create vibrant color variations

### Performance Optimizations
- Double buffering for smooth arendering
- Viewport culling to avoid rendering off-screen elements
- Efficient color blending algorithm

## Controls

- **Arrow Keys**: Pan the view
  - Up: Pan up
  - Down: Pan down
  - Left: Pan left
  - Right: Pan right
- **W/S**: Adjust simulation speed
  - W: Increase speed
  - S: Decrease speed
- **A/D**: Change ant size
  - A: Decrease ant size
  - D: Increase ant size
- **R**: Reset simulation
- **ESC**: Exit program

## Building

### Prerequisites
- Windows operating system
- C++ compiler (tested with g++)
- GDI32 library (included with Windows)

### Compilation
```bash
g++ -o langton_ant.exe langton_ant.cpp -lgdi32
```

## Implementation Details

### Key Components

1. **LangtonAnt Class**
   - Manages ant position, direction, and behavior
   - Handles cell color changes
   - Implements complex movement patterns

2. **Rendering System**
   - Double buffering for smooth animation
   - Cell color blending
   - Ant glow effects
   - Viewport management

3. **Color Management**
   - Base color palette
   - Color blending algorithm
   - Vibrant color variations

### Data Structures

- **Grid**: 2D vector of integers representing cell colors
- **Ants**: Vector of LangtonAnt pointers
- **Color Palettes**: Arrays of COLORREF values

## Future Enhancements

Potential improvements for future versions:
- Save/load simulation states
- Custom color palettes
- More complex ant behaviors
- Pattern analysis tools
- Performance metrics display
- Network multiplayer support

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is open source and available under the MIT License.

## Acknowledgments

- Based on the original Langton's Ant cellular automaton
- Inspired by various implementations and visualizations
- Thanks to the Windows GDI32 library for graphics support
