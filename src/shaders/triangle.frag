#version 460 core
out vec4 FragColor;

in vec4 vertexColor;

uniform float t;

void main()
{
	FragColor = vertexColor * vec4((sin(t) + 1.f) / 2, (cos(t) + 1.f) / 2, (-sin(t) + 1.f), 1.f);
}