# Soft Bodies

🏀 Real-time soft body simulation sandbox.

## Features

- Basic joint and spring physics integration.
- Rectangle-joint collisions.
- Basic OpenGL 3.3 renderer.
- Dear ImGui integration.

## Gallery

https://github.com/rayferric/soft-bodies/assets/63957587/2b22cf53-9e2a-48cb-9713-e47bbd9da866

<div style="display: flex; flex-direction: row; flex-wrap: wrap; justify-content: center;">
    <img style="width: 400px; margin: 20px;" src="./docs/vehicle.gif" />
</div>

## Getting Started

### Prerequisites

- **[CMake](https://cmake.org/download/#latest)**

### Build Instructions

```
cmake -B ./build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --config Release
```

The desired binary will be generated in `./build/bin` directory.

### Controls

> LMB over air: spawn joint
>
> LMB over joint: spawn spring
> 
> LMB while spawning spring: attach spring or abort

> RMB over air: spawn rectangle
> 
> RMB while spawning rectangle: apply size or angle

> RMB over joint: delete joint
> 
> RMB over rectangle: delete rectangle

## About

### Authors

- Ray Ferric (**[rayferric](https://github.com/rayferric)**)

### License

This project is licensed under the MIT License. See the **[LICENSE](LICENSE)** file for details.
