#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D txt;
uniform bool useColor;
uniform vec3 color;

void main()
{
	if (useColor)
        FragColor = vec4(color, 1.0);
	else
		FragColor = texture(txt, TexCoords);
}