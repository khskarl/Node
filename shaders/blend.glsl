#version 330 core
in vec2 texCoords;
out vec3 color;

uniform sampler2D inputTex;

void main()
{
	//color = texture(inputTex, vec2(texCoords.x, (texCoords.y))).xyz;
	color = vec3(1, 1, 1);
}