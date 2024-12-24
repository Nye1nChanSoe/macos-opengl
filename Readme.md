# OpenGL 4.1 Project with Premake5

This project purposefully uses **OpenGL 4.1**, which is the latest supported OpenGL version on **macOS**.

It ensures compatibility and performance on **Apple silicon** hardware.
<br/>

- **Language**: C++
- **Dependencies**:
  <br/>

  Install Apple's Command Line Tools

  ```bash
  xcode-select --install
  ```

  - [GLFW](https://www.glfw.org/) for windowing and input handling.
  - [GLM](https://github.com/g-truc/glm) for mathematics
  - [spdlog](https://github.com/gabime/spdlog) for fast C++ logging

    ```bash
    brew install glfw
    brew install glm
    brew install spdlog
    ```

## **Generating build configurations**

- [Premake5](https://premake.github.io/)
  ```bash
  brew install premake
  ```

## Example Usage

```bash
premake5 gmake2
premake5 xcode4
premake5 vs2022
```

### Easiest way to build and run the project

```bash
premake5 gmake2
cd build
make
cd ..
./bin/{build-config}/macos-opengl
```

Alternatively, you can run the shell script to build and execute the project in one go

```bash
./generate.sh
```

  <br/>

## **Dependencies Added**

- [ImGui](https://github.com/ocornut/imgui/tree/docking) v1.91.7 WIP

<br/>

## **TODO List**

1. [x] **Integrate SpdLog for Debugging**

2. [ ] **BufferLayout class for VAO, VBO, EBO**

   - Create and manage layouts for buffer objects used in rendering.

3. [ ] **Imgui overlay for textures, shaders, object manipulation, and input via mouse clicks**

   - Develop an overlay for managing assets and object inputs interactively.

4. [ ] **Intermediate shader concepts for planet shadows**

   - Implement shadow mapping and improve dynamic lighting effects.

5. [ ] **Physics Simulation (real-time physics engines)**

   - Simulate gravitational interactions and orbital dynamics.

6. [ ] **Sound and Audio Integration**

   - Add background sounds, spatial audio, and event-based sound effects.

7. [ ] **Scientific Data Integration**

   - Use real-world celestial datasets (e.g., NASA) for accurate simulations.

8. [ ] **Cross-Platform Compatibility**

   - Ensure the application runs smoothly on macOS, Windows, and Linux.

9. [ ] **Post-Processing Effects**

   - Add features like bloom, HDR, depth of field, and motion blur.

10. [ ] **Procedural Planet and Terrain Generation**
    - Create procedural planetary surfaces using noise algorithms.
