#version 330 core
in vec2 texCoords;
out vec3 color;

uniform sampler2D inputTex;

void main()
{	
	vec3 c = texture(inputTex, vec2(texCoords.x, texCoords.y)).xyz;
	float l = (c.x + c.y + c.z) / 3.0f;
	if (l >= 0.5f)
		l = 1;
	else
		l = 0;

	color = vec3(l, l, l);
}