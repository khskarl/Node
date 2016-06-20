#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include "GLshader.h"

bool LoadAllShaders();
unsigned GetProgram(unsigned index);
unsigned CreateFramebuffer(unsigned &texID);
void DrawFramebuffer(unsigned fboID, unsigned program, unsigned inputTex = 0);

#endif // OPENGL_UTILS_H