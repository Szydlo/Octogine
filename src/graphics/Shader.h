#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include "spdlog/spdlog.h"

#include <string>
#include <filesystem>
#include <fstream> 
#include <sstream>

namespace Octo
{
    class Shader
    {
        public:
            Shader(std::string vertexShader, std::string fragmentShader, std::string geomtryShader = "");
            ~Shader();

            void setBool(std::string name, bool value);
            void setInt(std::string name, int value);
            void setFloat(std::string name, float value);
            void setVec2(std::string name, glm::vec2 value);
            void setVec3(std::string name, glm::vec3 value);
            void setVec4(std::string name, glm::vec4 value);
            void setMat2(std::string name, glm::mat2 value);
            void setMat3(std::string name, glm::mat3 value);
            void setMat4(std::string name, glm::mat4 value);

            void bind();
            void unbind();

            unsigned int getID() { return m_Identity; }
        private:
            unsigned int m_Identity;

            void checkErrorShader(unsigned int shader, std::string path);
	        void checkErrorProgram(unsigned int shader);
    };
};