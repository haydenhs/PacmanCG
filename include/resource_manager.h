/*************************************************

Author: He Sun

Date: 2020-04-16

Description: A class of Resource Manager for Pacman game

 Code from LearnOpenGL Tutorial with modification.

**************************************************/

#ifndef PACMANCG_RESOURCE_MANAGER_H
#define PACMANCG_RESOURCE_MANAGER_H

// C++ STL
#include <map>
#include <string>
// System Headers
#include <GL/glew.h>
// Self-defined headers
#include "texture.h"
#include "shader.h"


// 资源管理类实现
// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
    // 储存的渲染器和纹理 Resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;

    // 加载渲染器和纹理
    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    // Loads (and generates) a texture from file
    static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);

    // 使用给定名字的渲染器和纹理
    // Retrieves a stored shader
    static Shader&   GetShader(std::string name);
    // Retrieves a stored texture
    static Texture2D& GetTexture(std::string name);

    // 清空 Properly de-allocates all loaded resources
    static void      Clear();
private:
    // Private constructor.
    ResourceManager() { }

    // 从文件加载渲染器和纹理
    // Loads and generates a shader from file
    static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif //PACMANCG_RESOURCE_MANAGER_H
