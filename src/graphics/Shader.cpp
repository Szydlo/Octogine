#include "Shader.h"

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
	glShaderSource(vID, 1, &vCodeP, NULL);
	glCompileShader(vID);
	checkErrorShader(vID, vertexShader);

	fragmentCode = fBuffer.str();
	const char* fCodeP = fragmentCode.c_str();

	fID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fID, 1, &fCodeP, NULL);
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
            glShaderSource(gID, 1, &gCodeP, NULL);
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

void Shader::bind()
{
    glUseProgram(m_Identity);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::checkErrorProgram(unsigned int shader)
{
	int success;
	char infoLog[1024];

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        
        spdlog::error("Shader error program {}", infoLog);
	}
}

void Shader::checkErrorShader(unsigned int shader, std::string path)
{
	int success;
	char infoLog[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
        spdlog::error("Shader problem on path {}", path);
		spdlog::error("Shader problem: {}", infoLog);
	}
}

void Shader::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(m_Identity, name.c_str()), (int)value);
}

void Shader::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(m_Identity, name.c_str()), value);
}

void Shader::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(m_Identity, name.c_str()), value);
}

void Shader::setVec2(std::string name, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(m_Identity, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(std::string name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(m_Identity, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(std::string name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(m_Identity, name.c_str()), 1, &value[0]);
}

void Shader::setMat2(std::string name, glm::mat2 value)
{
	glUniformMatrix2fv(glGetUniformLocation(m_Identity, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(std::string name, glm::mat3 value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_Identity, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_Identity, name.c_str()), 1, GL_FALSE, &value[0][0]);
}