#version 330 core
in vec2 texCoords;
out vec3 color;

uniform sampler2D inputTex;

void main()
{	
	vec3 c = texture(inputTex, vec2(texCoords.x, texCoords.y).xyz;
	
	if (c >= 0.5f)
		c = 1;
	else
		c = 0;

	color = c;
}