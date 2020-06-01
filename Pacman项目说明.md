## Pacman项目说明

### 第三方库
* OpenGL 3.3以上
* GLFW
* GLEW
* GLM

### 文件目录
./CMakeLists.txt：CMake配置文件

./main.cpp：主函数入口

./src：项目类的具体实现.cpp文件

./include/：项目所创建的.h头文件

* ./include/shader：项目所创建的shader着色器文件(.vs/.fs)

./resources/：项目用到的资源文件

* ./resources/textures：纹理.png图像
* ./resources/textures：关卡信息储存.lvl文件

### 参考内容说明
项目中，资源管理方法和加载方法主要参考了网络教程，类ResourceManager、类Renderer、类Texture2D为教程代码，仅作了少量改动。
