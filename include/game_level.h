/*************************************************

Author: He Sun

Date: 2020-04-16

Description: A class of Game Level for Pacman game

**************************************************/

#ifndef PACMANCG_GAME_LEVEL_H
#define PACMANCG_GAME_LEVEL_H

// C++ STL
#include <vector>
// System Headers
#include <GL/glew.h>
#include <glm/glm.hpp>
// Self-defined headers
#include "game_object.h"
#include "monster.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

// 每个关卡由砖块拼接的迷宫构成
class GameLevel
{
public:
    // 关卡中的资源: 砖块、得分标志和怪物
    std::vector<GameObject> Bricks;
    std::vector<GameObject> Dots;
    std::vector<MonsterObject> Monsters;

    // 构造函数
    GameLevel() { }

    // 从文件中加载关卡信息
    void    Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);

    // 渲染关卡场景
    void    Draw(SpriteRenderer &renderer);

    // 检测关卡是否完成
    GLboolean IsCompleted(GLuint currentScore);

    // 重置当前关卡
    void    Reset();

private:
    // 初始化关卡资源
    void    init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
    // 总分数
    GLuint totalScore;
};

#endif //PACMANCG_GAME_LEVEL_H
