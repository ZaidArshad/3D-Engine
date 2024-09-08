#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "shader.h"
#include "helper/logger.h"

Shader::Shader()
{
    program = glCreateProgram();
}

Status Shader::generateShader(const std::string &vPath, const std::string &fPath)
{
    std::string vShaderSource = parseShaderFile(vPath);
    std::string fShaderSource = parseShaderFile(fPath);

    Status status = STATUS_OK;

    GLuint vShader, fShader;
    status |= compileShader(vShader, vShaderSource, GL_VERTEX_SHADER);
    status |= compileShader(fShader, fShaderSource, GL_FRAGMENT_SHADER);
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return status;
}

GLuint Shader::getProgram() const
{
    return program;
}

void Shader::useShader()
{
    glUseProgram(program);
}

void Shader::deleteShader()
{
    glDeleteProgram(program);
}

void Shader::setFragmentColor(const Color &color)
{
    GLint location = glGetUniformLocation(program, "u_Color");
    glUniform4f(location, color.r, color.g, color.b, color.a);
}

Status Shader::compileShader(GLuint &shader, std::string &shaderSource, GLenum type)
{
    shader = glCreateShader(type);
    const char *src = shaderSource.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)_malloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        std::stringstream log;
        log << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader.\n"
            << message;
        Logger::log(log.str());
        glDeleteShader(shader);
        deleteShader();
        return STATUS_BAD;
    }

    return STATUS_OK;
}

std::string Shader::parseShaderFile(const std::string &path)
{
    std::ifstream stream(path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}