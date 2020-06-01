/*************************************************

Author: He Sun

Date: 2020-04-16

Description: A class of Game Object for Pacman game

**************************************************/

#ifndef PACMANCG_GAME_OBJECT_H
#define PACMANCG_GAME_OBJECT_H

// C++ STL
#include <GL/glew.h>
#include <glm/glm.hpp>
// Self-defined headers
#include "sprite_renderer.h"
#include "shader.h"
#include "texture.h"

// 游戏中物体的抽象类
class GameObject
{
public:
    // 物体的位置和移动信息
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;

    // 纹理图片
    Texture2D   Sprite;

    // 构造函数
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    // 渲染场景
    virtual void Draw(SpriteRenderer &renderer);
};


#endif //PACMANCG_GAME_OBJECT_H
