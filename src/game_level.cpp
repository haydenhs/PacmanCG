/*************************************************

Author: He Sun

Date: 2020-04-16

Description: Implementation of class game level for Pacman

**************************************************/

// C++ STL
#include <fstream>
#include <sstream>
// Self-defined headers
#include "game_level.h"

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
    // 清空之前的关卡资源
    this->Bricks.clear();
    this->Dots.clear();
    this->Monsters.clear();
    this->totalScore = 0;

    // 从对应文件中加载一个表示关卡场景的int型数组
    GLuint tileCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint>> tileData;
    if (fstream)
    {
        // 逐行读取
        while (std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

// 渲染关卡场景
void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
    for (GameObject &dot : this->Dots)
        if (!dot.Destroyed)
            dot.Draw(renderer);
    for (GameObject &mon : this->Monsters)
        if (!mon.Destroyed)
            mon.Draw(renderer);
}

// 吃豆进度统计
GLboolean GameLevel::IsCompleted(GLuint currentScore)
{
    return currentScore>=this->totalScore;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
    // 计算关卡尺寸信息
    GLuint height = tileData.size();
    GLuint width = tileData[0].size();
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / height;
    GLfloat dot_width = unit_width / 5, dot_height = unit_height / 5;
    GLfloat monster_width = unit_width * 2 / 3, monster_height = unit_height * 2 / 3;

    // 初始化关卡场景
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // 计算砖块的大小和位置
            if (tileData[y][x] == 1)
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else
            {
                // 计算豆子的大小和位置
                glm::vec2 pos(unit_width * x + (unit_width - dot_width) / 2, unit_height * y + (unit_height-dot_height) / 2);
                glm::vec2 size(dot_width, dot_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("dot"), glm::vec3(1.0f, 1.0f, 1.0f));
                obj.IsSolid = GL_FALSE;
                this->Dots.push_back(obj);
                this->totalScore++;
            }
            // 创建怪物
            if (tileData[y][x] == 2)
            {
                // 计算缩放视窗后怪物的大小和位置
                glm::vec2 pos(unit_width * x + (unit_width-monster_width)/2, unit_height * y + (unit_height-monster_height)/2);
                glm::vec2 size(monster_width, monster_height);
                glm::vec2 vel(50, 0);
                MonsterObject obj(pos, size, vel, ResourceManager::GetTexture("monster"));
                obj.IsSolid = GL_FALSE;
                this->Monsters.push_back(obj);
            }
        }
    }
}

void GameLevel::Reset() {
    // 重置已得分数
    for (GameObject &dot : this->Dots)
        dot.Destroyed = GL_FALSE;
    // 设置怪物为初始位置
    for (MonsterObject &mon : this->Monsters)
        mon.Reset();
}
