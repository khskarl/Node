#version 330 core
in vec2 texCoords;
out vec3 color;

void main()
{
	float x = abs(texCoords.x - 0.5f);
	float y = abs(texCoords.y - 0.5f);
	float d = sqrt(x * x + y * y); 
	if (d > 0.4f)
		d = 0;
	else {
		d = 1;
	}
	color = vec3(d, d, d);
}