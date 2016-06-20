#version 330 core
in vec2 texCoords;
out vec3 color;

uniform sampler2D inputTex;

void main()
{
	float x = texCoords.x + sin(texCoords.y);
	float y = texCoords.y + sin(texCoords.x);
	color = texture(inputTex, vec2(x, y)).xyz;
}