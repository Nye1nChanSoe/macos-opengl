# OpenGL4.1 Project with Premake5

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

### For $premake5 gmake2

```bash
premake5 gmake2
cd build
make
cd ..
./bin/{build-config}/macos-opengl
```

or run the shell script in one go

```bash
./build_and_run
```
