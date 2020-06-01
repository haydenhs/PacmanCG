/*************************************************

Author: He Sun

Date: 2020-04-16

Description: Implementation of class Sprite Renderer for Pacman

**************************************************/

#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(Shader &shader, GLboolean vamode)
{
    this->shader = shader;
    if (!vamode)
        this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    // 删除对应数组
    glDeleteVertexArrays(1, &this->quadVAO);
    glDeleteVertexArrays(1, &this->VAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    // 准备进行变换
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    // 平移+旋转+缩放
    model = glm::translate(model, glm::vec3(position, 0.0f));
    // 旋转时候需要将旋转中心先移动到物体中心
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    this->shader.SetMatrix4("model", model);

    // 设置渲染颜色 Render textured quad
    this->shader.SetVector3f("spriteColor", color);
    // 启用纹理
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    // 绘制顶点数组
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(GLfloat time, glm::vec2 position, GLfloat radius, GLfloat rotate, glm::vec3 color)
{

    // 生成动画当前时刻对应的数组
    GLuint pies = this->setAnimation(time);
    // 准备进行变换
    glm::mat4 projection = glm::ortho(0.0f, 900.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    this->shader.Use();
    this->shader.SetMatrix4("proj", projection);
    glm::mat4 model = glm::mat4(1.0f);
    // 平移+旋转+缩放
    model = glm::translate(model, glm::vec3(position.x, 600-position.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

    this->shader.SetMatrix4("model", model);
    // 设置物体颜色
    this->shader.SetVector3f("spriteColor", color);
    // 按照EBO索引进行绘制
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, pies*3, GL_UNSIGNED_INT, (GLvoid*) 0);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    // 配置VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
            // 位置     // 纹理
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    // 加载数组到缓存
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 绑定VAO
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint SpriteRenderer::setAnimation(GLfloat time)
{
    // 配置VAO/VBO
    GLuint VBO;
    // 计算动画当前时刻对应的数组信息
    GLfloat frequency = 8;
    GLuint offset = 15 + 15 * sin(time * frequency);
    GLuint degrees = 360 - 2 * offset;
    GLfloat vertices[degrees*2+2];
    GLuint indices[(degrees-1)*3];
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    for (int i = 0; i < degrees*2; i+=2)
    {
        vertices[i+2] = cos((i/2 + offset) * M_PI / 180.0);
        vertices[i+3] = sin((i/2 + offset) * M_PI / 180.0);
    }
    // 计算索引信息
    for (int i=0; i<(degrees-1)*3;i+=3)
    {
        indices[i] = 0;
        indices[i+1] = i/3+1;
        indices[i+2] = i/3+2;
    }
    // 生成缓冲数据
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    // 绑定结果
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return degrees-1;
}