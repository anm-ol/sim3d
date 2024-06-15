Certainly! Here's a draft for your README file based on the information we discussed:

---

# Sim3D

Welcome to the Sim3D! 
This is the successor to Sim2D which was written in Java and Processing.
This engine is designed to simulate particles in a 3D environment using OpenGL and C++. It allows for particle collision and movement, and includes a scene editor for setting up simulation parameters.

## Project Overview

The primary purpose of this particle simulation engine is to create particle simulations in C++ using OpenGL from scratch as much as possible, without relying on third-party libraries. The particles are represented as spheres and can interact with each other and with walls. The final goal is to add physics features like constraints, springs, etc., to turn it into a cloth simulation.

## Key Features

- Particle collision and movement
- Scene editor for setting up simulation parameters
- Different types of advanced lighting methods (currently includes Phong)
- GUI for the scene editor (to be added)

## Installation

To run the engine, you will need:
- OpenGL version 3
- C++ compiler

All dependencies required for compiling and running the engine are included in the `src/include` directory:
- GLM
- GLAD
- GLFW
- Dear ImGui

## Usage

To run the simulation:
1. Clone the repository.
2. Navigate to the directory containing the repository.
3. Compile and run the `main.cpp` file.
4. Set the engine state parameters (number of particles, their positions, wall positions, velocities, gravity, etc.) directly in the `main.cpp` file.
5. Use the following keyboard controls during simulation:
   - **WASD**: Camera movement
   - **Left Shift**: Toggle cursor visibility to interact with the GUI
   - **Space**: Move camera up
   - **Left Ctrl**: Move camera down
   - **Enter**: Play/pause simulation


## Contact

If you have any questions, suggestions, or issues, feel free to contact [anmolagrawal5704@example.com](mailto:anmolagrawal5704@gmail.com).
