#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GraphicObjects.h"

 //code to read shaders from sl file goes here'

class Shader
{
public:
    unsigned int ID;

    Shader() : ID(0)
    {}
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(std::string vertexPath, std::string fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath.c_str());
            fShaderFile.open(fragmentPath.c_str());
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const char* name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const char* name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const char* name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name), value);
    }

    void setVec3f(const char* name, glm::vec3& vector)
    {
        glUniform3f(glGetUniformLocation(ID, name), vector.x, vector.y, vector.z);
    }

    void setMatrix4f(const char* name, glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void setPointLight(const std::string& name, pointLight& light)
    {
        glUniform3f(glGetUniformLocation(ID, (name + ".pos").c_str()), light.pos.x, light.pos.y, light.pos.z);
        glUniform3f(glGetUniformLocation(ID, (name + ".color").c_str()), light.color.x, light.color.y, light.color.z);
        glUniform1f(glGetUniformLocation(ID, (name + ".intensity").c_str()), light.intensity);
        glUniform1f(glGetUniformLocation(ID, (name + ".attenuation").c_str()), light.attenuation);
    }

    void setLights(const std::string& name, std::vector<pointLight>& lights)
    {
        setInt("numlights", lights.size());
        for (int i = 0; i < lights.size(); i++)
        {
            setPointLight(name + "[" + std::to_string(i) + "]", lights[i]);
        }
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

//read and convert to string
//compile vertex shader
//compile frag shader
//link with program
//return program ID and display if any errors during compilation or linking