
# ECS Breakout Engine
![Status](https://img.shields.io/badge/Status-Educational_Prototype-yellow?style=for-the-badge)

> Data-Oriented C++ Game Engine Prototype.

I wanted to experiment and create a very simple 2d game engine using ECS arquitecture.

https://github.com/user-attachments/assets/1a8e8f14-97a0-4f64-af09-ae3c38757ca2

> Footage captured using original assets from *Balatro*.

---

## Project Structure

This is a source-only repository. Binary files, IDE configurations, and copyrighted assets have been excluded.

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
This project is  based on the great [Breakout Tutorial](https://learnopengl.com/In-Practice/2D-Game/Breakout) from LearnOpenGL.com. 

The original tutorial implements the game using Object-Oriented Programming. I wanted to follow the tutorial, but change the architecture.
####  Base Code 
I reused the following boilerplate code from the original tutorial :
 - `Shader`, `Texture`, and `ResourceManager` utility classes. 
 -  The original Level parsing logic (refactored to spawn Entities instead of Objects). 
 - The core gameplay rules and collision resolution math.



