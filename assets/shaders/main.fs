#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D txt;

uniform vec3 viewPos;
uniform bool useColor;
uniform vec3 color;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.diffuse;

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularStrength = 0.5;
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 specular = specularStrength * spec * light.diffuse;  

	return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calculateDirLight(dirLight, norm, viewDir);

	if (useColor)
		FragColor = vec4(color * result, 1.0);
	else
		FragColor = texture(txt, TexCoords);

}