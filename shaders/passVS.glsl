#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;

// Output data ; will be interpolated for each fragment.
out vec2 texCoords;

void main(){

	gl_Position = vec4(vPos, 1);

	texCoords = vUV;
}

