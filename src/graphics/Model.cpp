#include "Model.h"

using Octo::Model;

Model::Model(std::string path, bool useColor, glm::vec3 color)
 :  m_Shader("../../../assets/shaders/main.vs", "../../../assets/shaders/main.fs"),
	m_UseColor(useColor), m_Color(color)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace 			|
		aiProcess_ValidateDataStructure 	|
		aiProcess_JoinIdenticalVertices 	|
		aiProcess_ImproveCacheLocality 		|
		aiProcess_RemoveRedundantMaterials 	|
		aiProcess_FindDegenerates 			|
		aiProcess_FindInvalidData 			|
		aiProcess_GenUVCoords 				|
		aiProcess_TransformUVCoords 		|
		aiProcess_FindInstances 			|
		aiProcess_LimitBoneWeights 			|
		aiProcess_OptimizeMeshes 			|
		aiProcess_SplitByBoneCount 			|

		aiProcess_GenSmoothNormals 			|
		aiProcess_SplitLargeMeshes 			|
		aiProcess_Triangulate 				|
		aiProcess_SortByPType 				|
		0
	);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		spdlog::error("[MESH] bad path");

		return;
	}

    initMesh(scene->mRootNode, scene);
}

void Model::initMesh(aiNode* node, const aiScene* scene)
{
    for (unsigned int m = 0; m < node->mNumMeshes; m++)
    {
        auto& mesh = scene->mMeshes[node->mMeshes[m]];

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int v = 0; v < mesh->mNumVertices; v++)
        {
            Vertex vertex;

            if (mesh->HasPositions())
            {
                vertex.position[0] = mesh->mVertices[v].x;
				vertex.position[1] = mesh->mVertices[v].y;
				vertex.position[2] = mesh->mVertices[v].z;
            }

            if (mesh->HasTextureCoords(0))
			{
				vertex.txtCoord[0] = mesh->mTextureCoords[0][v].x;
				vertex.txtCoord[1] = mesh->mTextureCoords[0][v].y;
			}

			if (mesh->HasNormals()) {
				vertex.normal[0] = mesh->mNormals[v].x;
				vertex.normal[1] = mesh->mNormals[v].y;
				vertex.normal[2] = mesh->mNormals[v].z;
			}

            vertices.emplace_back(vertex);
        }

        for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
            for (unsigned int ind = 0; ind < mesh->mFaces[f].mNumIndices; ind++)
			{
				indices.push_back(mesh->mFaces[f].mIndices[ind]);
			}
        }
		
		m_Meshes.emplace_back(vertices, indices, "../../../assets/textures/uvs.jpg");
    }

    for (unsigned int c = 0; c < node->mNumChildren; c++) 
    {
        initMesh(node->mChildren[c], scene);
    }
}

void Model::draw()
{
    for (auto& mesh : m_Meshes)
    {
		m_Shader.bind();

		if (m_UseColor)
		{
			m_Shader.setBool("useColor", true);
			m_Shader.setVec3("color", m_Color);
		}
		else 
		{
			m_Shader.setBool("useColor", false);
		}

		m_Material.setShader(m_Shader);

		mesh.draw(m_Shader, m_Transform);
    }  
}