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

            void setBool(const std::string &name, bool value) const;
            void setInt(const std::string& name, int value) const;
            void setFloat(const std::string& name, float value) const;
            void setVec2(const std::string& name, const glm::vec2& value) const;
            void setVec3(const std::string& name, const glm::vec3& value) const;
            void setVec4(const std::string& name, const glm::vec4& value) const;
            void setMat2(const std::string& name, const glm::mat2& value) const;
            void setMat3(const std::string& name, const glm::mat3& value) const;
            void setMat4(const std::string& name, const glm::mat4& value) const;

            void bind() const;
            static void unbind();

            [[nodiscard]] unsigned int getID() const { return m_Identity; }
        private:
            unsigned int m_Identity;

            static void checkErrorShader(unsigned int shader, std::string path);
	        static void checkErrorProgram(unsigned int shader);
    };
};