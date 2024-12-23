# OpenGL 4.1 Project with Premake5

This project purposefully uses **OpenGL 4.1**, which is the latest supported OpenGL version on **macOS**.

It ensures compatibility and performance on **Apple silicon** hardware.

This project renders a solar system using **OpenGL 4.1**.

- **Language**: C++
- **Dependencies**:
  Install Apple's Command Line Tools

  ```bash
  xcode-select --install
  ```

  - [GLFW](https://www.glfw.org/) for windowing and input handling.
  - [GLM](https://github.com/g-truc/glm) for mathematics

    ```bash
    brew install glfw
    brew install glm
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

## **Dependencies Added**

- [ImGui](https://github.com/ocornut/imgui/tree/docking) v1.91.7 WIP
