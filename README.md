# Soft Bodies

🏀 real-time soft body simulation sandbox

## Features

- basic joint and spring physics integration
- rectangle-joint collisions
- basic OpenGL 3.3 renderer
- Dear ImGui integration

## Gallery

<div style="display: flex; flex-direction: row; flex-wrap: wrap; justify-content: center;">
    <video style="width: 400px; margin: 20px;" controls>
        <source src="./docs/soft-body.mp4" type="video/mp4" />
    </video>
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