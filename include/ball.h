/*************************************************

Author: He Sun

Date: 2020-04-16

Description: A class of ball-shape game object for Pacman

**************************************************/

#ifndef PACMANCG_BALL_H
#define PACMANCG_BALL_H

// System Headers
#include <GL/glew.h>
#include <glm/glm.hpp>
// Self-defined headers
#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"


class BallObject : public GameObject
{
public:
    // 构造函数
    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity);
    // 重设Pacman位置为起始位置
    void    Reset();
    // 绘制图形
    void    Draw(SpriteRenderer &renderer);
    // 开闭动画计时
    void    Mouth(GLfloat dt);
    // 球体半径
    GLfloat   Radius;
private:
    // 起始坐标
    glm::vec2 StartPos;
    // 计时器
    GLfloat timer = 0.0f;
};

#endif //PACMANCG_BALL_H
