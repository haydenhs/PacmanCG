/*************************************************

Author: He Sun

Date: 2020-04-16

Description: A class of Monster for Pacman game

**************************************************/

#ifndef PACMANCG_MONSTER_H
#define PACMANCG_MONSTER_H

// System Headers
#include <GL/glew.h>
#include <glm/glm.hpp>
// Self-defined headers
#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"

class MonsterObject : public GameObject
{
public:
    // 怪物的构造函数
    MonsterObject();
    MonsterObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite);

    // 怪物运动
    void      Move(GLfloat dt);

    // 重设怪物到初始状态
    void      Reset();

private:
    // 初始的位置和速度
    glm::vec2 StartPos;
    glm::vec2 StartVel;
};

#endif //PACMANCG_MONSTER_H
