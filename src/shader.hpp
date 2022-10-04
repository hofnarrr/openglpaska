#pragma once

#include <string>

#include <glad/glad.h>

class Shader {
public:
    unsigned int ID;

    Shader(const char *vtxShaderSrc, const char *fragShaderSrc);

    void use();

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setUInt(const std::string &name, unsigned int value);
    void set2UInt(const std::string &name, unsigned value1, unsigned value2);
    void setFloat(const std::string &name, float value);
    void setDouble(const std::string &name, double value);
    void set2Double(const std::string &name, double value1, double value2);
};
