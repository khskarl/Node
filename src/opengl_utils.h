#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include "GLshader.h"

bool LoadAllShaders();
unsigned GetProgram(unsigned index);
unsigned CreateFramebuffer();

#endif // OPENGL_UTILS_H