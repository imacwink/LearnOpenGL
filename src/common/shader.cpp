#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include <iostream>
#include <fstream>
#include <string.h>

// Sets the vertex shader of this program.
bool Shader::setVertexShader(std::string a_pathToShaderSourceFile)
{
    std::string shaderSource = this->loadShaderSource(a_pathToShaderSourceFile);
    this->mVertexShaderSource = shaderSource.c_str();

    // Create the new vertex shader.
    this->mVertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    // Add the shader code to the shader.
    glShaderSource(this->mVertexShaderID, 1, &this->mVertexShaderSource, NULL);
    glCompileShader(this->mVertexShaderID);

    if(!this->shaderCompiled(this->mVertexShaderID))
        return false;

    return true;
}

bool Shader::setFragmentShader(std::string a_pathToShaderSourceFile)
{
    std::string shaderSource = this->loadShaderSource(a_pathToShaderSourceFile);
    this->mFragmentShaderSource = shaderSource.c_str();

    // Create the new vertex shader.
    this->mFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Add the shader code to the shader.
    glShaderSource(this->mFragmentShaderID, 1, &this->mFragmentShaderSource, NULL);
    glCompileShader(this->mFragmentShaderID);

    if(!this->shaderCompiled(this->mFragmentShaderID))
        return false;

    return true;
}

int Shader::compile()
{
    this->mShaderProgram = glCreateProgram();

    // Attach the vertex and fragment shader.
    glAttachShader(this->mShaderProgram, this->mVertexShaderID);
    glAttachShader(this->mShaderProgram, this->mFragmentShaderID);
    
    // Compile the shader.
    glLinkProgram(this->mShaderProgram);

    int compiled;
    char infoLog[512];

    // Get the compile status.
    glGetProgramiv(this->mShaderProgram, GL_LINK_STATUS, &compiled);

    if(!compiled)
    {
        glGetShaderInfoLog(this->mShaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR: Could not compile shader program: " << infoLog << std::endl; 
    
        return -1;
    }

    // Delete the shaders.
    glDeleteShader(this->mVertexShaderID);
    glDeleteShader(this->mFragmentShaderID);

    return this->mShaderProgram;
}

void Shader::use()
{
    glUseProgram(this->mShaderProgram);
}

std::string Shader::loadShaderSource(std::string a_pathToShaderSourceFile)
{
    std::ifstream fileReader;
    fileReader.open(a_pathToShaderSourceFile, std::ios::binary);

    // Make sure to check that the file is open.
    if(fileReader.is_open())
    {
        // Set the 'cursor' at the beginning of the file.
        fileReader.seekg(0, std::ios::beg);

        std::string line;
        std::string output;
        while (std::getline(fileReader, line))
        {
            output.append(line);
            output.append("\n");
        }

        // Add a null terminator at the end of the string.
        output.append("\0");

        return output;
    }
    else
    {
        std::cerr << "Could not open file." << std::endl;
    }

    return "";
}

bool Shader::shaderCompiled(unsigned int a_id)
{
    int compiled;
    char infoLog[512];

    // Get the compile status.
    glGetShaderiv(a_id, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        glGetShaderInfoLog(a_id, 512, NULL, infoLog);
        std::cerr << "ERROR: Could not compile shader: " << infoLog << std::endl; 
    
        return false;
    }

    return true;
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(mShaderProgram, name.c_str()), value); 
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, &value[0]); 
}

void Shader::setVec2(const std::string &name, float x, float y) const
{ 
    glUniform2f(glGetUniformLocation(mShaderProgram, name.c_str()), x, y); 
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, &value[0]); 
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{ 
    glUniform3f(glGetUniformLocation(mShaderProgram, name.c_str()), x, y, z); 
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, &value[0]); 
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{ 
    glUniform4f(glGetUniformLocation(mShaderProgram, name.c_str()), x, y, z, w); 
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}