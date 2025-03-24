#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "spdlog/spdlog.h"

using Octo::Shader;

Shader::Shader(std::string vertexShader, std::string fragmentShader, std::string geomtryShader)
{
    std::string vertexCode, fragmentCode, geometryCode;

    if (vertexShader.empty() || fragmentShader.empty()) 
    {
        spdlog::error("[Shader] Path is empty");
        return;
    }

    if (!std::filesystem::exists(vertexShader))
    {
        spdlog::error("[Shader] Path like that doesn't exist {}", vertexShader);
        return;
    }

    if (!std::filesystem::exists(fragmentShader))
    {
        spdlog::error("[Shader] Path like that doesn't exist {}", fragmentShader);
        return;
    }

    std::ifstream vFile(vertexShader);
	std::ifstream fFile(fragmentShader);
    std::stringstream vBuffer, fBuffer;

	vBuffer << vFile.rdbuf();
	fBuffer << fFile.rdbuf();
	vFile.close();
	fFile.close();

    unsigned int vID, fID;
	int gID = -1;

    vertexCode = vBuffer.str();
	const char* vCodeP = vertexCode.c_str();

	vID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vID, 1, &vCodeP, nullptr);
	glCompileShader(vID);
	checkErrorShader(vID, vertexShader);

	fragmentCode = fBuffer.str();
	const char* fCodeP = fragmentCode.c_str();

	fID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fID, 1, &fCodeP, nullptr);
	glCompileShader(fID);
	checkErrorShader(fID, fragmentCode);

    m_Identity = glCreateProgram();
	glAttachShader(m_Identity, vID);
	glAttachShader(m_Identity, fID);

	if (!geomtryShader.empty())
	{
        if (std::filesystem::exists(geomtryShader))
        {
            std::ifstream gFile(geomtryShader);
            std::stringstream gBuffer;
            gBuffer << gFile.rdbuf();
            gFile.close();

            std::string gCode = gBuffer.str();
            const char* gCodeP = gCode.c_str();

            gID = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gID, 1, &gCodeP, nullptr);
            glCompileShader(gID);
            checkErrorShader(gID, geomtryShader);
            glAttachShader(m_Identity, gID);
        }
        else 
        {
            spdlog::error("[Shader] Path like that doesn't exist {}", geomtryShader);
        }
	}

    glLinkProgram(m_Identity);
	checkErrorProgram(m_Identity);

	glDeleteShader(vID);
	glDeleteShader(fID);

	if (gID >= 0)
		glDeleteShader(gID);
}

Shader::~Shader()
{
    glDeleteProgram(m_Identity);
}

void Shader::bind() const
{
    glUseProgram(m_Identity);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::checkErrorProgram(const unsigned int shader)
{
	int success;

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        
        spdlog::error("Shader error program {}", infoLog);
	}
}

void Shader::checkErrorShader(const unsigned int shader, std::string path)
{
	int success;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		spdlog::error("Shader problem on path {}", path);
		spdlog::error("Shader problem: {}", infoLog);
	}
}

void Shader::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(m_Identity, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(m_Identity, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(m_Identity, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_Identity, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_Identity, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_Identity, name.c_str()), 1, &value[0]);
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_Identity, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_Identity, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_Identity, name.c_str()), 1, GL_FALSE, &value[0][0]);
}