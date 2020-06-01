/*************************************************

Author: He Sun

Date: 2020-04-16

Description: Implementation of class Monster for Pacman game

**************************************************/

#include "monster.h"

MonsterObject::MonsterObject()
        : GameObject(){ }

MonsterObject::MonsterObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, Texture2D sprite)
        :  GameObject(pos, size, sprite, glm::vec3(1.0f), velocity)
{
    this->StartPos = pos;
    this->StartVel = velocity;
}

// 沿速度矢量方向移动怪物
void MonsterObject::Move(GLfloat dt)
{
    this->Position = this->Position + dt * this->Velocity;
}

// 重设怪物的起始坐标和速度
void MonsterObject::Reset()
{
    this->Position = StartPos;
    this->Velocity = StartVel;
}