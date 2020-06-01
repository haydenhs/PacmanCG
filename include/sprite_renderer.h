/*************************************************

Author: He Sun

Date: 2020-04-16

Description: Class Sprite Renderer for Pacman

**************************************************/

#ifndef PACMANCG_SPRITE_RENDERER_H
#define PACMANCG_SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class SpriteRenderer
{
public:
    // 构造函数 Constructor (inits shaders/shapes)
    SpriteRenderer(Shader &shader, GLboolean vamode=GL_FALSE);

    // 析构函数 Destructor
    ~SpriteRenderer();

    // 绘制纹理图像 Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

    // 绘制普通几何图像 Renders a vanilla figure, no texture
    void DrawSprite(GLfloat time, glm::vec2 position, GLfloat radius, GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:

    // 渲染器 Render state
    Shader shader;
    // 顶点数组对象 VAOs
    GLuint quadVAO;
    GLuint VAO;

    // 初始化渲染数据
    void initRenderData();

    // 初始化动画数据
    GLuint setAnimation(GLfloat time);
};

#endif //PACMANCG_SPRITE_RENDERER_H
