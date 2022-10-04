#include <iostream>
#include <string>

#include "shader.hpp"

unsigned int shaderProg, vtxShader, fragShader;

Shader::Shader(const char *vtxShaderSrc, const char *fragShaderSrc) {
    // error checking stuff
    int success;
    const int infolog_size = 512;
    char infolog[infolog_size];

    // create vertex shader object and compile shader
    unsigned int vtxShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtxShader, 1, &vtxShaderSrc, NULL);
    glCompileShader(vtxShader);
    glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vtxShader, infolog_size, NULL, infolog);
        std::cerr << "couldn't compile vertex shader :(\n" << infolog
            << std::endl;
    }

    // create fragment shader object and compile shader
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, infolog_size, NULL, infolog);
        std::cerr << "couldn't compile fragment shader :(\n" << infolog
            << std::endl;
    }

    // build shader program
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vtxShader);
    glAttachShader(shaderProg, fragShader);
    glLinkProgram(shaderProg);
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProg, infolog_size, NULL, infolog);
        std::cerr << "couldn't create shader program :(\n" << infolog
            << std::endl;
    }
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
}


void Shader::setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(shaderProg, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(shaderProg, name.c_str()), value);
}

void Shader::setUInt(const std::string &name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(shaderProg, name.c_str()), value);
}

void Shader::set2UInt(const std::string &name, unsigned int value1, unsigned int value2) {
    glUniform2ui(glGetUniformLocation(shaderProg, name.c_str()), value1, value2);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(shaderProg, name.c_str()), value);
}

void Shader::setDouble(const std::string &name, double value) {
    glUniform1d(glGetUniformLocation(shaderProg, name.c_str()), value);
}

void Shader::set2Double(const std::string &name, double value1, double value2) {
    glUniform2d(glGetUniformLocation(shaderProg, name.c_str()), value1, value2);
}

void Shader::use() {
    glUseProgram(shaderProg);
}
