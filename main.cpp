/*************************************************

Author: He Sun

Date: 2020-04-16

Description: main function for the project

**************************************************/

// C++ STL
#include <iostream>
// System Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Self-defined headers
#include "game.h"
#include "resource_manager.h"


using namespace std;

// 处理键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// 屏幕显示参数 screen settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 600;

// 构造游戏类
Game Pacman(SCR_WIDTH, SCR_HEIGHT);


// 当用户改变窗口的大小的时候，调整视口
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // 初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // 新建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pacman - CG project", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 准备GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    // 设置键盘读取
    glfwSetKeyCallback(window, key_callback);

    // OpenGL设置
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 初始化类
    Pacman.Init();

    // 设置时间步长
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // 对窗口注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 渲染循环
    while(!glfwWindowShouldClose(window))
    {
        // 计算帧时间
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // 游戏处理输入，更新状态
        Pacman.ProcessInput(deltaTime);
        Pacman.Update(deltaTime);

        //选择清空的颜色RGBA
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Pacman.Render();

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
    }

    // 删除加载的资源
    ResourceManager::Clear();

    // 释放资源
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 输入esc直接退出
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Pacman.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Pacman.Keys[key] = GL_FALSE;
    }
}