# OpenGL4.1 Project with Premake5

- **Language**: C++
- **Dependencies**:
  - [GLFW](https://www.glfw.org/) for windowing and input handling.
  - [GLM](https://github.com/g-truc/glm) for mathematics (matrices and vectors).
    ```bash
    brew install glfw
    brew install glm
    ```

## **Generating build configurations**

- [Premake5](https://premake.github.io/)
  ```bash
  brew install premake
  ```

## Usage

```bash
premake5 gmake2
premake5 xcode4
premake5 vs2022
```

### premake5 gmake2

```bash
premake5 gmake2
cd build
make
cd ..
./bin/{build-config}/macos-opengl
```

or

```bash
./build_and_run
```
