#pragma once

#define GL_SILENCE_DEPRECATION

#include <string>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

GLuint LoadTexture(const std::string &path);
