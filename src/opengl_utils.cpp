// opengl_utils.cpp
#include "opengl_utils.h"
#include <iostream>

// DOES ABSOLUTELY FREAKIN' NOTHING :D

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

GLuint vboID;
GLuint uvBufferID;

static const GLfloat gQuadBufferData[] = {
   1.0f, -1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f,
   1.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f,
   1.0f, 1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f,
};


static const GLfloat gTexCoordBufferData[] = {
   1.0f, 0, 
   0, 0,
   1.0f, 1.0f,
   0, 1.0f,
   1.0f, 1.0f, 
   0, 0,
};

bool LoadAllShaders() {
	bool result = true;

	unsigned i = 0;
	for(const char* shaderPath : _paths) {
		std::cout << shaderPath << "\n";
		GLuint program = LoadShader("shaders/passVS.glsl", shaderPath);

		if (program == 0) {
			result = false;
			break;
		}

		_programs[i++] = program;

		std::cout << "ID: " << program << "\n";
		glUseProgram(program);
	}

//////////////////
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Generate 1 buffer, put the resulting identifier in vboID
	glGenBuffers(1, &vboID);
	// The following commands will talk about our 'vboID' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(gQuadBufferData), gQuadBufferData, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vboID
	glGenBuffers(1, &uvBufferID);
	// The following commands will talk about our 'vboID' buffer
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(gTexCoordBufferData), gTexCoordBufferData, GL_STATIC_DRAW);

/////////////

	return result;
}

unsigned GetProgram(unsigned index) {
	return _programs[index];
}

unsigned CreateFramebuffer(unsigned &texID) {
	// The framebuffer, which regroups 0, 1, or more textures, and 
	// 0 or 1 depth buffer.
	GLuint fboID = 0;
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	// std::cout << fboID << "\n";

	// The texture we're going to render to
	GLuint targetTextureID;
	glGenTextures(1, &targetTextureID);
	glBindTexture(GL_TEXTURE_2D, targetTextureID);
	texID = targetTextureID;
	// std::cout << targetTextureID << "\n";

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Good bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glGenerateMipmap(GL_TEXTURE_2D);

	// Why would you do that filtering
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set "targetTextureID" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTextureID, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "WTF FRAMEBUFFEEEEEEEEEER\n";
	
	std::cout << "SAVED\n";
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	return fboID;
}

void DrawFramebuffer(unsigned fboID, unsigned program, unsigned inputTex) {
	program = _programs[program];
	std::cout << program << " LALAL \n";

	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	glViewport(0, 0, 512, 512); 
	if (fboID >= 3)
		glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
	else
		glClearColor(0.1f, 0.5f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	if (inputTex != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputTex);
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(fboID, 0);
	}

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glVertexAttribPointer(
	   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}