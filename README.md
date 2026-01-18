# ⚛️ 2D Physics Simulation Engine

A high-performance real-time physics sandbox built with **C++** and **SFML 3.0**. This engine simulates rigid body dynamics, implementing impulse-based collision resolution, gravity, and momentum conservation for multi-object systems.

![C++](https://img.shields.io/badge/C%2B%2B-17%2F20-blue) ![SFML](https://img.shields.io/badge/SFML-3.0-green) ![Physics](https://img.shields.io/badge/Focus-Rigid%20Body%20Dynamics-orange)

## 🚀 Key Features

*   **Impulse-Based Collision Resolution**: Implements elastic collision logic where kinetic energy and momentum are exchanged between objects based on mass and velocity vectors.
*   **Environmental Physics**:
    *   **Gravity**: Constant downward acceleration ($9.8m/s^2$ scaled).
    *   **Boundary Constraints**: Floor, ceiling, and wall collision detection with coefficient of restitution (bounciness).
*   **Interactive Simulation**: Users can dynamically instantiate new physics objects into the active simulation loop via mouse interaction.
*   **Modern C++ & SFML 3**: Utilizes modern event polling patterns (`std::optional`) and vector-based initialization introduced in SFML 3.0.

## ⚙️ How It Works

The engine uses a **discrete time-step simulation loop** to approximate real-world physics:

1.  **Update Phase**:
    *   Apply gravity to velocity (`v_y += g * dt`).
    *   Update positions based on velocity (`pos += v * dt`).
    *   Check for boundary collisions (walls/floor) and invert velocity with a "bounce factor" (0.7).

2.  **Collision Phase ($O(N^2)$)**:
    *   Checks every object against every other object.
    *   If circles overlap, they are separated to prevent "sinking."
    *   **Impulse Calculation**: Calculates the exact force vector required to separate them based on their masses and incoming angles.

## 🛠 Prerequisites

*   **C++ Compiler**: GCC (g++), Clang, or MSVC (C++17 standard or higher).
*   **SFML 3.0**: This project uses the latest SFML 3.0 API (e.g., `window.pollEvent()` returning `std::optional`).

## 💻 Build & Run Instructions


Ensure your SFML headers/libs are in the standard path or updated in the command below:

```bash
# Compile
g++ main.cpp -o physics_engine -lsfml-graphics -lsfml-window -lsfml-system

# Run
./physics_engine
