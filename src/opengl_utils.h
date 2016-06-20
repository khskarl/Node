#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include "GLshader.h"

bool LoadAllShaders();
unsigned GetProgram(unsigned index);
unsigned CreateFramebuffer();
void DrawFramebuffer(unsigned fboID, unsigned program);

#endif // OPENGL_UTILS_H