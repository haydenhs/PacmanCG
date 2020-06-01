/*************************************************

Author: He Sun

Date: 2020-04-16

Description: Implementation of class ball for Pacman

**************************************************/

#include "ball.h"

BallObject::BallObject()
        : GameObject(), Radius(12.5f) { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity)
        :  GameObject(pos, glm::vec2(radius * 2, radius * 2),glm::vec3(1.0f), velocity), Radius(radius)
        {
            this->StartPos = pos;
        }

void BallObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->timer, this->Position, this->Radius, this->Rotation, glm::vec3(1, 1, 0));
}

// 运行计时，用于设置动画
void BallObject::Mouth(GLfloat dt)
{
    this->timer += dt;
}

// 重设起始位置
void BallObject::Reset()
{
    this->Position = this->StartPos;
}
