#version 330 core
in vec2 texCoords;
out vec3 color;

void main()
{
	color = vec3(texCoords.x, texCoords.y, 1.0f);
}