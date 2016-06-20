// opengl_utils.cpp
#include "opengl_utils.h"
#include <iostream>

// DOES ABSOLUTELY FREAKIN' NOTHING :D

void CreateFramebuffer() {
	// The framebuffer, which regroups 0, 1, or more textures, and 
	// 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "WTF FRAMEBUFFEEEEEEEEEER\n";
	
	std::cout << "SAVED\n";
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
static const char* _paths[] = 
{
	"shaders/circle.glsl",
	"shaders/blend.glsl",
	"shaders/threshold.glsl",
};

static unsigned _programs[] = 
{
	0,
	0,
	0,
};

bool LoadAllShaders() {
	bool result = true;

	unsigned i = 0;
	for(const char* shaderPath : _paths) {
		GLuint program = LoadShader("shaders/passVS.glsl", shaderPath);

		if (program == 0) {
			result = false;
			break;
		}

		_programs[i++] = program;

		glUseProgram(program);
	}
	CreateFramebuffer();

	return result;
}

unsigned GetProgram(unsigned index) {
	return _programs[index];
}
