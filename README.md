
# ECS Breakout Engine
![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)<br />
![Graphics](https://img.shields.io/badge/graphics-OpenGL%20%2F%20SDL3-green.svg)<br />
![Architecture](https://img.shields.io/badge/architecture-ECS-orange.svg)<br />
![Status](https://img.shields.io/badge/Status-Educational_Prototype-yellow?style=for-the-badge)

> **A Data-Oriented C++ Game Engine Prototype.**

This project is a custom implementation of the **Entity Component System (ECS)** architectural pattern. It was developed to study **memory management** and **cache locality** by refactoring a traditional Object-Oriented codebase into a Data-Oriented one.

The goal was to experiment with a data-oriented architecture and apply low-level OpenGL concepts to create a lightweight but practical 2D game engine.

https://github.com/user-attachments/assets/0e299e49-822c-4287-b528-b51c797b2c6b
> **Tech Demo:** Footage captured using original assets from *Balatro*.

---

##  Architecture

Traditional game development often relies on Object-Oriented Programming, where game entities are objects scattered across the heap. This causes frequent cache misses due to "pointer chasing."

This engine takes a different approach. It is built around a **Data-Oriented Design**, prioritizing memory layout and access patterns over object abstraction.

### 1. ECS Implementation
I implemented a lightweight ECS from scratch to understand the internals:
* **Component Storage:** Components are stored in **contiguous static arrays** (Component Pools) rather than dynamic vectors or scattered heap allocations. While individual components are structs (AoS), storing them tightly packed ensures **O(1)** access and excellent **spatial locality** compared to traditional pointer-based object lists.
* **Decoupled Logic:** Game logic is separated into stateless Systems (`PhysicsSystem`, `RenderSystem`, `AudioSystem`) that iterate linearly over these packed component arrays.
* **Zero Allocation Loop:** The gameplay loop avoids dynamic memory allocation (`new`/`malloc`) during runtime to prevent heap fragmentation and frame spikes.

### 2. Rendering Pipeline
* **Low-Level OpenGL:** Direct state management using `glDrawArrays`, VAOs, and VBOs, avoiding the overhead of high-level engines.
* **Shader Management:** Custom shader compilation pipeline supporting geometry shaders and dynamic uniform updates.

---

## Project Structure

This is a **source-only repository** intended for architectural review. Binary files, IDE configurations, and copyrighted assets have been excluded.

```text
.
├── include
│   ├── breakout/       # Level loading and game-specific parsing logic
│   ├── ecs/
│   │   ├── components/ # Plain Old Data structs (POD). No logic, just data.
│   │   │   ├── PhysicsComponents.hpp
│   │   │   ├── RenderComponents.hpp
│   │   │   └── ...
│   │   ├── systems/    # Stateless logic processors that iterate over component arrays
│   │   │   ├── PhysicsSystem.hpp
│   │   │   ├── RenderSystem.hpp
│   │   │   └── ...
│   │   ├── EntityComponent.hpp # Core Sparse Set implementation
│   │   ├── ECS.hpp     # Core Entity & Component Manager definitions
│   │   └── Game.hpp    # Scene initialization and entity composition
│   ├── render/         # OpenGL wrappers (Shader, Texture, ResourceManager)
│   └── ...             # External lib headers (miniaudio, stb_image, glad)
├── shaders/            # GLSL source code (Vertex & Fragment shaders)
└── src
    ├── breakout/       # Level parsing implementation
    ├── ecs/            # Implementation of Systems and Managers
    ├── render/         # OpenGL state management implementation
    ├── main.cpp        # Application Entry Point, Time Step Loop & SDL3 Setup

```

---
## Attribution & Credits
This project is an **architectural refactoring study** based on the great [Breakout Tutorial](https://learnopengl.com/In-Practice/2D-Game/Breakout) from **LearnOpenGL.com**. 

The original tutorial implements the game using traditional **Object-Oriented Programming**, where game entities are objects holding their own data and logic. **My contribution was to  re-engineer the core architecture:** 
- **From OOP to ECS:** I stripped the logic out of the classes and implemented a custom **Entity Component System**. 
- **Memory Layout:** Instead of scattered using objects, I implemented contiguous memory arrays for components to study cache locality. *
- **Systems:** I rewrote the game loop logic (`Ball` movement, `Collisions`, `Rendering`) to work as stateless systems iterating over data, rather than object methods. 
####  Base Code 
I reused the following boilerplate code from the original tutorial :
 - `Shader`, `Texture`, and `ResourceManager` utility classes. 
 -  The original Level parsing logic (refactored to spawn Entities instead of Objects). 
 - The core gameplay rules and collision resolution math.



