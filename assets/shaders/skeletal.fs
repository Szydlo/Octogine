#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
flat in ivec4 BoneIds;
in vec4 Weights;

uniform sampler2D txt;
uniform bool useColor;
uniform vec3 color;

void main()
{
	vec3 color = vec3(0.0, 0.0, 1.0);

	for (int i = 0; i < 4; i++)
	{
		if (BoneIds[i] == 13) 
		{
			if (Weights[i] >= 0.7)
				color = vec3(1.0, 0.0, 0.0) * Weights[i];
			else if (Weights[i] >= 0.4 && Weights[i] <= 0.6)
				color = vec3(0.0, 1.0, 0.0) * Weights[i];
			else if (Weights[i] >= 0.1)
				color = vec3(1.0, 1.0, 0.0) * Weights[i];
		}

	}

	FragColor = vec4(color, 1.0);

	//if (useColor)
      //  FragColor = vec4(color, 1.0);
	//else
	//	FragColor = texture(txt, TexCoords);
}