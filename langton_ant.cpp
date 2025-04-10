#include <windows.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>  // Add math functions
#include <random> // Add random number generation
#include <ctime>  // Add time functions for seeding

const int WINDOW_SIZE = 800;
const int GRID_SIZE = 100;
const int CELL_SIZE = WINDOW_SIZE / GRID_SIZE-2;
const int MIN_ANT_SIZE = 1;  // Minimum ant size (pixel)
const int MAX_ANT_SIZE = 8;  // Maximum ant size
const int DEFAULT_ANT_SIZE = 1;  // Default ant size
const int NUM_ANTS = 100;  // Fixed number of ants

// Enhanced colors
const COLORREF BACKGROUND_COLOR = RGB(20, 20, 30);  // Dark background for better contrast
const COLORREF ANT_COLORS[] = {  // More vibrant colors for ants
    RGB(255, 50, 50),    // Bright red
    RGB(50, 255, 50),    // Bright green
    RGB(50, 50, 255),    // Bright blue
    RGB(255, 255, 50),   // Bright yellow
    RGB(255, 50, 255),   // Bright magenta
    RGB(50, 255, 255),   // Bright cyan
    RGB(255, 150, 50),   // Bright orange
    RGB(200, 50, 255),   // Bright purple
    RGB(50, 255, 150),   // Bright mint
    RGB(255, 150, 150)   // Bright pink
};
const int NUM_ANT_COLORS = sizeof(ANT_COLORS) / sizeof(ANT_COLORS[0]);

// RGB color palette for cells - more vibrant and diverse
const int NUM_COLORS = 12;  // More colors for more interesting patterns
const COLORREF CELL_COLORS[NUM_COLORS] = {
    RGB(255, 255, 255),  // White
    RGB(255, 50, 50),    // Bright red
    RGB(50, 255, 50),    // Bright green
    RGB(50, 50, 255),    // Bright blue
    RGB(255, 255, 50),   // Bright yellow
    RGB(255, 50, 255),   // Bright magenta
    RGB(50, 255, 255),   // Bright cyan
    RGB(255, 150, 50),   // Bright orange
    RGB(200, 50, 255),   // Bright purple
    RGB(50, 255, 150),   // Bright mint
    RGB(255, 150, 150),  // Bright pink
    RGB(200, 200, 50)    // Bright gold
};

// Speed settings
const int MAX_SPEED = 20000;  // Maximum speed
const int DEFAULT_SPEED = MAX_SPEED;  // Start at maximum speed

// Function to create a vibrant color variation
COLORREF VibrantColor(COLORREF color) {
    int r = GetRValue(color);
    int g = GetGValue(color);
    int b = GetBValue(color);
    
    // Increase saturation by boosting the dominant component
    if (r >= g && r >= b) {
        r = std::min(255, r + 50);  // Increased boost
    } else if (g >= r && g >= b) {
        g = std::min(255, g + 50);  // Increased boost
    } else {
        b = std::min(255, b + 50);  // Increased boost
    }
    
    return RGB(r, g, b);
}

// Function to blend two colors with enhanced blending
COLORREF BlendColors(COLORREF color1, COLORREF color2) {
    int r1 = GetRValue(color1);
    int g1 = GetGValue(color1);
    int b1 = GetBValue(color1);
    
    int r2 = GetRValue(color2);
    int g2 = GetGValue(color2);
    int b2 = GetBValue(color2);
    
    // Enhanced blending: weighted average with stronger bias toward brighter colors
    int r = (r1 * 4 + r2 * 1) / 5;
    int g = (g1 * 4 + g2 * 1) / 5;
    int b = (b1 * 4 + b2 * 1) / 5;
    
    // Ensure values don't exceed 255
    r = std::min(255, r);
    g = std::min(255, g);
    b = std::min(255, b);
    
    return RGB(r, g, b);
}

class LangtonAnt {
private:
    static std::vector<std::vector<int>> sharedGrid;  // Shared grid for all ants
    int antX, antY;
    int direction; // 0: up, 1: right, 2: down, 3: left
    int colorIndex; // Current color index
    int stepsSinceLastTurn; // Track steps since last turn
    int turnCounter; // Counter for complex movement patterns

public:
    LangtonAnt() : antX(GRID_SIZE/2), 
                   antY(GRID_SIZE/2), 
                   direction(0),
                   colorIndex(0),
                   stepsSinceLastTurn(0),
                   turnCounter(0) {
        // Initialize shared grid if it's empty
        if (sharedGrid.empty()) {
            sharedGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
        }
    }

    // Constructor with random position and direction
    LangtonAnt(bool randomize) {
        // Initialize shared grid if it's empty
        if (sharedGrid.empty()) {
            sharedGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
        }
        
        if (randomize) {
            // Create a random number generator
            std::random_device rd;
            std::mt19937 gen(rd());
            
            // Random position
            std::uniform_int_distribution<> posDist(0, GRID_SIZE - 1);
            antX = posDist(gen);
            antY = posDist(gen);
            
            // Random direction
            std::uniform_int_distribution<> dirDist(0, 3);
            direction = dirDist(gen);
            
            // Random initial color index
            std::uniform_int_distribution<> colorDist(0, NUM_COLORS - 1);
            colorIndex = colorDist(gen);
            
            // Initialize counters
            stepsSinceLastTurn = 0;
            turnCounter = 0;
        } else {
            // Default values
            antX = GRID_SIZE/2;
            antY = GRID_SIZE/2;
            direction = 0;
            colorIndex = 0;
            stepsSinceLastTurn = 0;
            turnCounter = 0;
        }
    }

    void step() {
        // Get current cell color index
        int currentColor = sharedGrid[antY][antX];
        
        // Increment step counter
        stepsSinceLastTurn++;
        turnCounter = (turnCounter + 1) % 1000; // Cycle through patterns

        // Complex turning behavior based on cell color and position
        if (currentColor == 0) {  // White cell
            // Basic rule: turn right on white
            direction = (direction + 1) % 4;
        } else {
            // Enhanced rule: turn left on colored cells, but with variations
            if (turnCounter < 200) {
                // Normal left turn
                direction = (direction + 3) % 4;
            } else if (turnCounter < 400) {
                // Occasionally turn right instead (10% chance)
                if (rand() % 10 == 0) {
                    direction = (direction + 1) % 4;
                } else {
                    direction = (direction + 3) % 4;
                }
            } else if (turnCounter < 600) {
                // Occasionally turn 180 degrees (5% chance)
                if (rand() % 20 == 0) {
                    direction = (direction + 2) % 4;
                } else {
                    direction = (direction + 3) % 4;
                }
            } else if (turnCounter < 800) {
                // Follow a more complex pattern based on position
                if ((antX + antY) % 3 == 0) {
                    direction = (direction + 1) % 4; // Turn right
                } else {
                    direction = (direction + 3) % 4; // Turn left
                }
            } else {
                // Return to normal behavior
                direction = (direction + 3) % 4;
            }
        }

        // Change the color of current cell with enhanced coloring
        int newColor = (currentColor + 1) % NUM_COLORS;
        
        // Occasionally create a more vibrant color (20% chance)
        if (rand() % 5 == 0 && newColor > 0) {
            // Create a vibrant variation of the color
            COLORREF baseColor = CELL_COLORS[newColor];
            COLORREF vibrantColor = VibrantColor(baseColor);
            
            // Store the vibrant color in a special way (we'll handle this in rendering)
            sharedGrid[antY][antX] = newColor + NUM_COLORS; // Add NUM_COLORS to indicate vibrant
        } else {
            sharedGrid[antY][antX] = newColor;
        }

        // Calculate new position
        int newX = antX;
        int newY = antY;
        
        // Move forward
        switch (direction) {
            case 0: newY--; break; // Up
            case 1: newX++; break; // Right
            case 2: newY++; break; // Down
            case 3: newX--; break; // Left
        }

        // Only move if the new position is within the grid boundaries
        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
            antX = newX;
            antY = newY;
        } else {
            // If we hit a boundary, turn around
            direction = (direction + 2) % 4;
        }
    }

    int getWindowWidth() const { return GRID_SIZE * CELL_SIZE; }
    int getWindowHeight() const { return GRID_SIZE * CELL_SIZE; }
    
    // Add getters for ant position and direction
    int getAntX() const { return antX; }
    int getAntY() const { return antY; }
    int getDirection() const { return direction; }
    
    // Add getter for grid
    static const std::vector<std::vector<int>>& getGrid() { return sharedGrid; }
    
    // Static method to clear the shared grid
    static void clearGrid() {
        sharedGrid = std::vector<std::vector<int>>(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    }
};

// Initialize the static member
std::vector<std::vector<int>> LangtonAnt::sharedGrid;

// Global variables
std::vector<LangtonAnt*> ants;  // Vector to store multiple ants
bool running = true;
int speed = DEFAULT_SPEED; // Start at maximum speed
int antSize = DEFAULT_ANT_SIZE; // Current ant size

// Viewport offset for panning
int viewportOffsetX = 0;
int viewportOffsetY = 0;
const int PAN_SPEED = 10; // Pixels to move per key press

// Buffer variables
HDC bufferDC = NULL;
HBITMAP bufferBitmap = NULL;
int bufferWidth = 0;
int bufferHeight = 0;

// Function to create or resize buffers
void CreateOrResizeBuffers(HWND hwnd, int width, int height) {
    HDC hdc = GetDC(hwnd);
    
    // Delete old buffers if they exist
    if (bufferDC) {
        DeleteDC(bufferDC);
        DeleteObject(bufferBitmap);
    }
    
    // Create new buffers
    bufferDC = CreateCompatibleDC(hdc);
    bufferBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(bufferDC, bufferBitmap);
    
    // Store new dimensions
    bufferWidth = width;
    bufferHeight = height;
    
    ReleaseDC(hwnd, hdc);
    
    // Clear the buffer
    RECT rect = {0, 0, width, height};
    HBRUSH bgBrush = CreateSolidBrush(BACKGROUND_COLOR);
    FillRect(bufferDC, &rect, bgBrush);
    DeleteObject(bgBrush);
}

// Function to clean up buffers
void CleanupBuffers() {
    if (bufferDC) {
        DeleteDC(bufferDC);
        DeleteObject(bufferBitmap);
        bufferDC = NULL;
        bufferBitmap = NULL;
    }
}

// Function to add a new ant
void AddAnt(bool randomize = true) {
    LangtonAnt* newAnt = new LangtonAnt(randomize);
    
    // If randomizing, ensure the ant doesn't start at the same position as existing ants
    if (randomize && !ants.empty()) {
        bool positionValid = false;
        int maxAttempts = 100; // Prevent infinite loop
        int attempts = 0;
        
        while (!positionValid && attempts < maxAttempts) {
            positionValid = true;
            
            // Check if the new ant's position overlaps with any existing ant
            for (auto ant : ants) {
                if (newAnt->getAntX() == ant->getAntX() && 
                    newAnt->getAntY() == ant->getAntY()) {
                    positionValid = false;
                    break;
                }
            }
            
            // If position is invalid, create a new ant with a different random position
            if (!positionValid) {
                delete newAnt;
                newAnt = new LangtonAnt(true);
                attempts++;
            }
        }
    }
    
    ants.push_back(newAnt);
}

// Function to remove all ants
void ClearAnts() {
    for (auto ant : ants) {
        delete ant;
    }
    ants.clear();
}

// Function to reset ants
void ResetAnts() {
    ClearAnts();
    LangtonAnt::clearGrid();  // Clear the shared grid
    for (int i = 0; i < NUM_ANTS; i++) {
        AddAnt(true);
    }
    viewportOffsetX = 0;
    viewportOffsetY = 0;
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            running = false;
            CleanupBuffers();
            PostQuitMessage(0);
            return 0;

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            CreateOrResizeBuffers(hwnd, width, height);
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Draw cells with colors
            const std::vector<std::vector<int>>& grid = LangtonAnt::getGrid();
            
            // Clear the buffer
            RECT rect = {0, 0, bufferWidth, bufferHeight};
            HBRUSH bgBrush = CreateSolidBrush(BACKGROUND_COLOR);
            FillRect(bufferDC, &rect, bgBrush);
            DeleteObject(bgBrush);
            
            // First pass: draw all cells
            for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                    int colorIdx = grid[y][x];
                    if (colorIdx > 0) {  // Only draw non-white cells
                        RECT cellRect = {
                            x * CELL_SIZE + viewportOffsetX,
                            y * CELL_SIZE + viewportOffsetY,
                            (x + 1) * CELL_SIZE + viewportOffsetX,
                            (y + 1) * CELL_SIZE + viewportOffsetY
                        };
                        
                        // Only draw if the cell is visible in the viewport
                        if (cellRect.right > 0 && cellRect.left < bufferWidth &&
                            cellRect.bottom > 0 && cellRect.top < bufferHeight) {
                            
                            // Handle vibrant colors
                            COLORREF cellColor;
                            if (colorIdx >= NUM_COLORS) {
                                // This is a vibrant color
                                cellColor = VibrantColor(CELL_COLORS[colorIdx - NUM_COLORS]);
                            } else {
                                cellColor = CELL_COLORS[colorIdx];
                            }
                            
                            HBRUSH cellBrush = CreateSolidBrush(cellColor);
                            FillRect(bufferDC, &cellRect, cellBrush);
                            DeleteObject(cellBrush);
                        }
                    }
                }
            }
            
            // Second pass: blend overlapping colors
            for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                    int colorIdx = grid[y][x];
                    if (colorIdx > 0) {  // Only process non-white cells
                        // Check neighboring cells
                        COLORREF currentColor;
                        if (colorIdx >= NUM_COLORS) {
                            currentColor = VibrantColor(CELL_COLORS[colorIdx - NUM_COLORS]);
                        } else {
                            currentColor = CELL_COLORS[colorIdx];
                        }
                        
                        COLORREF blendedColor = currentColor;
                        bool needsBlending = false;
                        
                        // Check right neighbor
                        if (x < GRID_SIZE - 1 && grid[y][x+1] > 0) {
                            COLORREF neighborColor;
                            if (grid[y][x+1] >= NUM_COLORS) {
                                neighborColor = VibrantColor(CELL_COLORS[grid[y][x+1] - NUM_COLORS]);
                            } else {
                                neighborColor = CELL_COLORS[grid[y][x+1]];
                            }
                            blendedColor = BlendColors(blendedColor, neighborColor);
                            needsBlending = true;
                        }
                        
                        // Check bottom neighbor
                        if (y < GRID_SIZE - 1 && grid[y+1][x] > 0) {
                            COLORREF neighborColor;
                            if (grid[y+1][x] >= NUM_COLORS) {
                                neighborColor = VibrantColor(CELL_COLORS[grid[y+1][x] - NUM_COLORS]);
                            } else {
                                neighborColor = CELL_COLORS[grid[y+1][x]];
                            }
                            blendedColor = BlendColors(blendedColor, neighborColor);
                            needsBlending = true;
                        }
                        
                        // If blending occurred, redraw the cell
                        if (needsBlending) {
                            RECT cellRect = {
                                x * CELL_SIZE + viewportOffsetX,
                                y * CELL_SIZE + viewportOffsetY,
                                (x + 1) * CELL_SIZE + viewportOffsetX,
                                (y + 1) * CELL_SIZE + viewportOffsetY
                            };
                            
                            // Only draw if the cell is visible in the viewport
                            if (cellRect.right > 0 && cellRect.left < bufferWidth &&
                                cellRect.bottom > 0 && cellRect.top < bufferHeight) {
                                HBRUSH cellBrush = CreateSolidBrush(blendedColor);
                                FillRect(bufferDC, &cellRect, cellBrush);
                                DeleteObject(cellBrush);
                            }
                        }
                    }
                }
            }
            
            // Draw all ants with their respective colors and enhanced visuals
            for (size_t i = 0; i < ants.size(); i++) {
                int antX = ants[i]->getAntX() * CELL_SIZE + CELL_SIZE/2 + viewportOffsetX;
                int antY = ants[i]->getAntY() * CELL_SIZE + CELL_SIZE/2 + viewportOffsetY;
                
                // Only draw ant if it's visible in the viewport
                if (antX > 0 && antX < bufferWidth && antY > 0 && antY < bufferHeight) {
                    COLORREF antColor = ANT_COLORS[i % NUM_ANT_COLORS];
                    
                    // Create a more pronounced glow effect for the ant
                    if (antSize > 1) {
                        // Outer glow
                        RECT outerGlowRect = {
                            antX - antSize * 2,
                            antY - antSize * 2,
                            antX + antSize * 2,
                            antY + antSize * 2
                        };
                        
                        // Create a semi-transparent brush for the outer glow
                        HBRUSH outerGlowBrush = CreateSolidBrush(antColor);
                        FillRect(bufferDC, &outerGlowRect, outerGlowBrush);
                        DeleteObject(outerGlowBrush);
                        
                        // Inner glow
                        RECT innerGlowRect = {
                            antX - antSize,
                            antY - antSize,
                            antX + antSize,
                            antY + antSize
                        };
                        
                        // Create a more opaque brush for the inner glow
                        HBRUSH innerGlowBrush = CreateSolidBrush(VibrantColor(antColor));
                        FillRect(bufferDC, &innerGlowRect, innerGlowBrush);
                        DeleteObject(innerGlowBrush);
                    }
                    
                    // Draw the ant itself
                    if (antSize == 1) {
                        // Draw as a single pixel with a small glow
                        SetPixel(bufferDC, antX, antY, VibrantColor(antColor));
                    } else {
                        // Draw as a square with a more defined shape
                        RECT antRect = {
                            antX - antSize/2,
                            antY - antSize/2,
                            antX + antSize/2,
                            antY + antSize/2
                        };
                        HBRUSH antBrush = CreateSolidBrush(VibrantColor(antColor));
                        FillRect(bufferDC, &antRect, antBrush);
                        DeleteObject(antBrush);
                    }
                }
            }
            
            // Copy the buffer to the screen
            BitBlt(hdc, 0, 0, bufferWidth, bufferHeight, bufferDC, 0, 0, SRCCOPY);
            
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_KEYDOWN:
            switch (wParam) {
                case VK_ESCAPE:
                    running = false;
                    PostQuitMessage(0);
                    break;
                case VK_UP:
                    // Pan up
                    viewportOffsetY -= PAN_SPEED;
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case VK_DOWN:
                    // Pan down
                    viewportOffsetY += PAN_SPEED;
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case VK_LEFT:
                    // Pan left
                    viewportOffsetX -= PAN_SPEED;
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case VK_RIGHT:
                    // Pan right
                    viewportOffsetX += PAN_SPEED;
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case 'W':  // Speed up
                    speed = std::min(MAX_SPEED, speed + 1);
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case 'S':  // Slow down
                    speed = std::max(1, speed - 1);
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case 'A':  // Decrease ant size
                    antSize = std::max(MIN_ANT_SIZE, antSize - 1);
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case 'D':  // Increase ant size
                    antSize = std::min(MAX_ANT_SIZE, antSize + 1);
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                case 'R':  // Reset with new random position
                    ResetAnts();
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Seed the random number generator with current time
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Create all ants with random positions
    ResetAnts();

    // Register the window class
    const char* CLASS_NAME = "LangtonAnt";
    
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    
    RegisterClassA(&wc);

    // Calculate window size to fit the grid
    RECT windowRect = {0, 0, ants[0]->getWindowWidth(), ants[0]->getWindowHeight()};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the window
    HWND hwnd = CreateWindowA(
        CLASS_NAME,                // Window class
        "Langton's Ant - RGB Blend (Arrows=Pan, W/S=Speed, A/D=Ant Size, R=Reset)",  // Window text
        WS_OVERLAPPEDWINDOW,      // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, // Position
        windowRect.right - windowRect.left,    // Width
        windowRect.bottom - windowRect.top,    // Height
        NULL,                      // Parent window    
        NULL,                      // Menu
        hInstance,                 // Instance handle
        NULL                       // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    
    // Initialize the buffer with the window size
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    CreateOrResizeBuffers(hwnd, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

    // Message loop
    MSG msg = {};
    DWORD lastUpdateTime = GetTickCount();
    const DWORD baseInterval = 50; // Base interval in milliseconds

    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Update simulation at variable speed
        DWORD currentTime = GetTickCount();
        if (currentTime - lastUpdateTime >= baseInterval / speed) {
            for (auto ant : ants) {
                ant->step();
            }
            InvalidateRect(hwnd, NULL, FALSE);
            lastUpdateTime = currentTime;
        }
    }

    // Cleanup
    ClearAnts();

    return 0;
} 