#version 330 core
in vec2 texCoords;
out vec3 color;

uniform sampler2D inputTex;

void main()
{
	float x = texCoords.x + cos(texCoords.y);
	float y = texCoords.y + cos(texCoords.x);
	color = texture(inputTex, vec2(x, y)).xyz;
}