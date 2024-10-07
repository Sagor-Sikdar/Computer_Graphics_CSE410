# CSE 410 - Computer Graphics

This repository contains solutions for the assignments of the CSE 410 - Computer Graphics course. Each assignment focuses on different aspects of computer graphics and includes OpenGL-based implementations. Below is a brief description of each assignment.

---

## Tech Stack

- **Language**: C++
- **Graphics Library**: OpenGL
- **Windowing and Input**: GLUT (OpenGL Utility Toolkit)
- **Image Output**: BMP format
- **Development Environment**: Any C++ compiler supporting OpenGL (e.g., g++ with OpenGL libraries)

---

## Assignment 1 - OpenGL

### Overview
This assignment implements a fully controllable camera system and a gun model, followed by bubble animations. It utilizes OpenGL to achieve basic 3D transformations and interactions.

### Features
1. **Fully Controllable Camera**
   - Control movement and rotation of the camera using keyboard keys.
   - 6 movement operations and 6 rotation operations.
   
2. **Gun Model**
   - Model a gun using basic shapes without OpenGL library functions.
   - Implement joint rotation and firing mechanics.
   
3. **Bubble Animation**
   - Simulate bubbles on a 2D plane using vector-based movement and collision handling.

---

## Assignment 2 - Curves

### Overview
This assignment focuses on drawing and updating 2D Hermite curves. It uses control points to generate curves and allows for dynamic updates.

### Features
1. **Curve Drawing**
   - Add control points via left-click and draw a closed 2D Hermite curve with the right-click.
   
2. **Curve Update**
   - Update existing control points and smoothly animate points along the curve.
   
3. **Geometry Toggle**
   - Toggle the display of control points and the geometry of the curve using the keyboard.
   - 
---

## Assignment 3 - Raster-Based Graphics Pipeline

### Overview
In this assignment, we develop a simplified version of the raster-based graphics pipeline used in OpenGL. The program generates a BMP image after applying a series of transformations and projections to 3D models.

### Features
1. **Modeling Transformation**
   - Parse `scene.txt` to apply scaling, rotation, and translation transformations.
   
2. **View Transformation**
   - Implement camera view transformations using a `gluLookAt` style approach.
   
3. **Projection Transformation**
   - Generate projection matrix and perform clipping for the scene.
   
4. **Scan Conversion with Z-buffer**
   - Apply z-buffer algorithm for rasterizing the triangles in the 3D scene and render them into a BMP image.

---

## Assignment 4 - Ray Tracer

### Overview
This assignment involves building a ray-tracing engine to render images of 3D scenes, considering light interactions such as reflection, diffuse, and specular effects.

### Features
1. **OpenGL Interface**
   - Draw 3D objects (sphere, pyramid, checkerboard) in an OpenGL window.
   
2. **Ray Tracing**
   - Implement ray generation from the camera and perform object intersection calculations.
   
3. **Lighting and Shading**
   - Calculate ambient, diffuse, and specular components for object shading.
   
4. **Reflection and Recursion**
   - Implement recursive ray tracing for handling reflections.
   
5. **Spotlight and Distance-Based Light Intensity**
   - Add spotlights with a cutoff angle and decrease light intensity based on distance.

---

### Compilation and Execution

Each assignment is implemented in C++ using OpenGL. The code is organized into separate directories for each assignment, with their respective source files.

### Requirements
- OpenGL and GLUT libraries
- C++ compiler

## Acknowledgments
- Institution: Bangladesh University of Engineering and Technology (BUET)
- Course: CSE 410 - Computer Graphics
