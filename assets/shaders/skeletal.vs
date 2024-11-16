#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec4 aBoneIds; 
layout (location = 4) in vec4 aWeights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
flat out ivec4 BoneIds;
out vec4 Weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLU = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    TexCoords = aTexCoords;
    BoneIds = aBoneIds;
    Weights = aWeights;
    
    vec4 totalPosition = vec4(0.0f);

    for (int i = 0; i < MAX_BONE_INFLU; i++)
    {
        if (aBoneIds[i] == -1) continue;
        if (aBoneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(aPos, 1.0);
            break;
        }

        vec4 localPosition = finalBonesMatrices[BoneIds[i]] * vec4(aPos, 1.0f);
       //vec4 localPosition = mat4(1) * vec4(aPos, 1.0f);
       
        totalPosition += localPosition * aWeights[i];
    }
    
    gl_Position = projection * (view * model) * totalPosition;
    //gl_Position = projection * (view * model) * vec4(aPos, 1.0f);
}