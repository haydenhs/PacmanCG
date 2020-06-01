/*************************************************

Author: He Sun

Date: 2020-04-16

Description: Implementation of class Game for Pacman

**************************************************/

// C++ STL
#include <iostream>
#include <string>
// Self-defined headers
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball.h"
#include "monster.h"

// 游戏用指针
SpriteRenderer  *Renderer;
SpriteRenderer *AniRenderer;
BallObject      *Agent;

// 速度方向
glm::vec2 direction[4] = {
        glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
};

Game::Game(GLuint width, GLuint height)
        : State(GAME_MENU), Keys(), Width(width), Height(height), Score(0)
{

}

Game::~Game()
{

}

void Game::Init()
{
    // 加载着色器
    ResourceManager::LoadShader("../include/shaders/sprite.vs", "../include/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("../include/shaders/vanilla.vs", "../include/shaders/vanilla.fs", nullptr, "vanilla");
    // 创建投影矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                      static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    // 设置着色器和纹理
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("vanilla").SetMatrix4("proj", projection);
    // 创建渲染控制器
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    AniRenderer = new SpriteRenderer(ResourceManager::GetShader("vanilla"), GL_TRUE);
    // 加载纹理
    ResourceManager::LoadTexture("../resources/textures/pac-man.png", GL_TRUE, "agent");
    ResourceManager::LoadTexture("../resources/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("../resources/textures/block.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("../resources/textures/dot.png", GL_TRUE, "dot");
    ResourceManager::LoadTexture("../resources/textures/start_menu.png", GL_FALSE, "start_menu");
    ResourceManager::LoadTexture("../resources/textures/win.png", GL_FALSE, "win");
    ResourceManager::LoadTexture("../resources/textures/lose.png", GL_FALSE, "lose");
    ResourceManager::LoadTexture("../resources/textures/ghost.png", GL_TRUE, "monster");
    // 加载关卡
    GameLevel one;
    one.Load("../resources/levels/test.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;

    // 加载Pacman
    glm::vec2 agentPos = glm::vec2(100, 100);
    glm::vec2 agentVel = glm::vec2(60, 60);
    GLuint agentRadius = 15;
    Agent = new BallObject(agentPos, agentRadius, agentVel);
}

void Game::Update(GLfloat dt)
{
    if (this->State==GAME_ACTIVE)
    {
        // 检测碰撞
        this->DoCollisions();

        // 更新monster的位置，碰壁处理
        for (MonsterObject &mon : this->Levels[this->Level].Monsters)
        {
            mon.Move(dt);
        }

        // 更新Pacman的动画
        Agent->Mouth(dt);
    }

    // 判断游戏是否结束
    if (this->Levels[this->Level].IsCompleted(this->Score))
        this->State = GAME_WIN;
}


void Game::ProcessInput(GLfloat dt)
{
    // 进入欢迎菜单
    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_SPACE])
        {
            this->State = GAME_ACTIVE;
        }
    }

    // 失败后重启游戏
    if (this->State == GAME_LOSE)
    {
        if (this->Keys[GLFW_KEY_SPACE])
        {
            this->ResetGame();
        }
    }

    // 游戏模式
    if (this->State == GAME_ACTIVE)
    {
        // 计算按键单个时间移动步长
        GLfloat step = Agent->Velocity.x * dt;
        // 上下左右移动
        if (this->Keys[GLFW_KEY_A])
        {
            if (Agent->Position.x >= 0)
                Agent->Position.x -= step;
            Agent->Rotation = 180;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Agent->Position.x <= this->Width - Agent->Size.x)
                Agent->Position.x += step;
            Agent->Rotation = 0;
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (Agent->Position.y >= 0)
                Agent->Position.y -= step;
            Agent->Rotation = 90;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (Agent->Position.y <= this->Height - Agent->Size.y)
                Agent->Position.y += step;
            Agent->Rotation = 270;
        }
    }
}

void Game::Render()
{
    // 游戏模式
    if(this->State == GAME_ACTIVE)
    {
        // 绘制背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                             glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

        // 绘制关卡(包括墙砖和豆子以及怪兽)
        this->Levels[this->Level].Draw(*Renderer);

        // 绘制Pacman
        Agent->Draw(*AniRenderer);
    }
    else if (this->State == GAME_WIN)
    {
        // 绘制胜利场景
        Renderer->DrawSprite(ResourceManager::GetTexture("win"),
                             glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
    }
    else if (this->State == GAME_LOSE)
    {
        // 绘制失败场景
        Renderer->DrawSprite(ResourceManager::GetTexture("lose"),
                             glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
    }
    else
    {
        // 绘制开始界面
        Renderer->DrawSprite(ResourceManager::GetTexture("start_menu"),
                             glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
    }
}

void Game::DoCollisions()
{
    // 处理砖块的碰撞
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {

        if (box.IsSolid && !box.Destroyed)
        {
            // 检测砖块与人物之间的碰撞
            BallCollision collision = CheckCollision(*Agent, box);
            if(std::get<0>(collision))
            {
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                {
                    // 计算重叠面积
                    GLfloat penetration = Agent->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        Agent->Position.x += penetration;
                    else
                        Agent->Position.x -= penetration;
                }
                else // 垂直方向碰撞
                {
                    // 计算重叠面积
                    GLfloat penetration = Agent->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        Agent->Position.y -= penetration;
                    else
                        Agent->Position.y += penetration;
                }
            }

            // 检测砖块与怪物的碰撞并自动改变方向
            for (MonsterObject &mon : this->Levels[this->Level].Monsters)
            {
                if (!mon.Destroyed)
                {
                    // 首先解除碰撞，分四种情况讨论
                    BoxCollision collision = CheckCollision(mon, box);
                    if(std::get<0>(collision))
                    {
                        Direction dir = std::get<1>(collision);
                        if (dir == LEFT || dir == RIGHT) // 发生水平方向碰撞
                        {
                            if (dir == LEFT)
                                mon.Position.x += 2*(box.Position.x + box.Size.x - mon.Position.x); // 右移
                            else
                                mon.Position.x -= 2*(mon.Position.x + mon.Size.x - box.Position.x); // 左移
                        }
                        else // 发生垂直方向碰撞
                        {
                            if (dir == UP)
                                mon.Position.y -= 2*(mon.Position.y + mon.Size.y - box.Position.y); // 上移
                            else
                                mon.Position.y += 2*(box.Position.y + box.Size.y - mon.Position.y); // 下移
                        }

                        // 之后重设速度方向为上下左右中的任一方向
                        int opt = rand()%4;
                        mon.Velocity = glm::length(mon.Velocity) * direction[opt];
                    }
                }
            }
        }
    }

    // 处理pacman吃豆情况
    for (GameObject &dot : this->Levels[this->Level].Dots)
    {
        if (!dot.Destroyed)
        {
            BallCollision collision = CheckCollision(*Agent, dot);
            if(std::get<0>(collision))
            {
                // 设置被吃掉的豆子使其不再显示
                dot.Destroyed = GL_TRUE;
                this->Score++;
                std::cout << "Eat a pill, current score is " << this->Score << std::endl;
            }
        }
    }

    // 处理Pacman与monster的碰撞
    for (MonsterObject &mon : this->Levels[this->Level].Monsters)
    {
        if (!mon.Destroyed)
        {
            BallCollision collision = CheckCollision(*Agent, mon);
            if(std::get<0>(collision))
                this->State = GAME_LOSE;
        }
    }
}

// 根据矢量积运算确定碰撞方向
Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
            glm::vec2(0.0f, 1.0f),  // 上
            glm::vec2(1.0f, 0.0f),  // 右
            glm::vec2(0.0f, -1.0f), // 下
            glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    // 向量最大内积的方向为运动方向
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

// 检测球体与长方体的圆——AABB型碰撞
BallCollision Game::CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // 获取圆的中心
    glm::vec2 center(one.Position);
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
            two.Position.x + aabb_half_extents.x,
            two.Position.y + aabb_half_extents.y
    );
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;

    // 计算相距矢量
    difference = closest - center;
    if (glm::length(difference) <= one.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

// 检测两个长方体的AABB型碰撞
BoxCollision Game::CheckCollision(GameObject &one, GameObject &two)
{
    // x轴方向是否发生坐标重叠
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // y轴方向是否发生坐标重叠
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;

    // 是否发生碰撞
    if (collisionX && collisionY)
        return std::make_tuple(GL_TRUE, VectorDirection(one.Velocity));
    else
        return std::make_tuple(GL_FALSE, UP);
}

void Game::ResetGame()
{
    // 重设所有场景信息和游戏状态
    this->Levels[this->Level].Reset();
    Agent->Reset();
    this->State = GAME_ACTIVE;
    this->Score = 0;
}