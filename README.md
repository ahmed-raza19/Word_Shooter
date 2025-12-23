# 🎯 WordShooter

**2D Tile-Based Pacman-Inspired Game Using OpenGL**  
FAST-NUCES | Fall 2024

---

## 📌 Project Overview

**WordShooter** is a C++ tile-based board game inspired by **Pacman**, developed to demonstrate **graphics programming, board design, and game logic** using OpenGL.  
The project focuses on **board rendering, tile management, and event handling**, creating a functional 2D game environment with walls, collectibles, and open spaces.

This project is an individual **Term Project** for the Department of Artificial Intelligence & Data Science, FAST‑NUCES.

---

## 🎯 Objectives

* Implement a tile-based game board in C++ using OpenGL.  
* Render walls, corners, pebbles, and special items with proper graphics primitives.  
* Handle open and blocked tiles for player movement and collisions.  
* Demonstrate modular programming with `Board.cpp`, `util.cpp`, and main game loop.  
* Learn practical OpenGL drawing, transformations, and color management.

---

## 🧩 Game Board

* **Board Size:** 28x36 tiles  
* **Tile Types:**  
  * Corners → Torus curves  
  * Walls → Straight lines  
  * Pebbles → Small circles  
  * Special Items → Colored balls  
* **Board Logic:**  
  * Board array stores tile type as enums  
  * `Board::IsOpen(x, y)` checks if a tile is walkable  
  * Rendering handled by `Board::Draw()` using OpenGL primitives  

---

## ⚙️ Core Functionalities

### 🎨 Graphics Rendering
* Corners → `Torus2d()`  
* Walls → `DrawLine()`  
* Pebbles → `DrawCircle()`  
* Solid balls / collectibles → `DrawCircle()` with larger radius  

### 🕹️ Game Mechanics
* Player moves through open tiles only  
* Collectibles placed on pebbles and special items  
* Ghost gates and walls restrict movement  
* Tile-based logic ensures collision detection

### 🖌️ Customization
* Adjustable tile size (`xcellsize`, `ycellsize`)  
* Custom colors for walls, collectibles, and special items  
* Easily extendable for new tile types or game mechanics

---

## 🛠️ Technologies Used

* C++  
* OpenGL / GLUT  
* GLEW  
* FreeImage (optional for textures)  
* Linux / Ubuntu  

---

## 📂 Project Structure

```text
.
├── main.cpp               # Game loop and input handling
├── Board.h                # Board class definition
├── Board.cpp              # Board rendering implementation
├── util.h                 # Graphics utilities header
├── util.cpp               # Graphics utilities implementation
├── install-libraries.sh   # Script to install OpenGL dependencies
├── Makefile               # Build instructions
└── README.md              # This file
