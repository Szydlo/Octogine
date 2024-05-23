#pragma once

#include "spdlog/spdlog.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

#include "../lighting/Material.h"

#include <map>
#include <vector>
#include <string>

namespace Octo
{

	static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}


    struct BoneInfo
    {
        int id;
        glm::mat4 offset;

    };

    class Model
    {
        public:
            Model(std::string path, bool useColor = false, glm::vec3 color = glm::vec3(1.0));

            void draw();
            void draw(Shader& shader);

            glm::mat4 getTransform() { return m_Transform; }
            void setTransform(glm::mat4 transform) { m_Transform = transform; }

            glm::vec3 getColor() { return m_Color; }
            void setColor(glm::vec3 color) { m_Color = color; }

            Material getMaterial() { return m_Material; }
            void setMaterial(Material material) { m_Material = material; }

            Shader& getShader() { return m_Shader; }
            std::vector<Mesh>& getMeshes() { return m_Meshes; }
                    
            void setVertexBoneData(Vertex& vertex, int boneID, float weight);
            void setVertexBoneDataToDefault(Vertex& vertex);

            void extractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
        private:
            void initMesh(aiNode* node, const aiScene* scene);

            Shader m_Shader;
            std::vector<Mesh> m_Meshes;

            glm::mat4 m_Transform = glm::mat4(1);

            bool m_UseColor = false;
            glm::vec3 m_Color = glm::vec3(1.0);

            Material m_Material;

            std::map<std::string, BoneInfo> mBoneInfoMap;
	        int mBoneCounter = 0;
    };
};