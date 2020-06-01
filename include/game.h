/*************************************************

Author: He Sun

Date: 2020-04-16

Description: A class of Pacman game

**************************************************/

#ifndef PACMANCG_GAME_H
#define PACMANCG_GAME_H

// C++ STL
#include <tuple>
// System Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Self-defined headers
#include "game_level.h"
#include "ball.h"

// 设置游戏状态
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSE
};

// 运动方向
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// 设置一个tuple储存碰撞信息
typedef std::tuple<GLboolean, Direction, glm::vec2> BallCollision;
typedef std::tuple<GLboolean, Direction> BoxCollision;

// 一个抽象游戏类
class Game
{
public:
    // 游戏状态
    GameState              State;
    // 键盘输入状态
    GLboolean              Keys[1024];
    // 游戏分辨率
    GLuint                 Width, Height;
    // 关卡设置
    std::vector<GameLevel> Levels;
    // 当前关卡
    GLuint                 Level;
    // 构造及析构函数
    Game(GLuint width, GLuint height);
    ~Game();

    // 初始化游戏
    void Init();

    // 处理键盘输入
    void ProcessInput(GLfloat dt);

    // 更新物体运动状态
    void Update(GLfloat dt);

    // 控制渲染
    void Render();

    // 碰撞检测
    void DoCollisions();

private:
    // 两种不同的碰撞类型检测
    BallCollision CheckCollision(BallObject &one, GameObject &two);
    BoxCollision CheckCollision(GameObject &one, GameObject &two);

    // 初始化关卡
    void ResetGame();

    // 当前得分
    GLuint Score;
};

#endif //PACMANCG_GAME_H
