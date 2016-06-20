#version 330 core
in vec2 texCoords;
out vec3 color;

uniform sampler2D inputTex;

void main()
{
	float x = texCoords.x + cos(texCoords.y * 50) * 0.05f;
	float y = texCoords.y;
	color = texture(inputTex, vec2(x, y)).xyz;
}