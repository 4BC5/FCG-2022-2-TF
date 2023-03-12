#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#define DEBUG 1
#define DRAW_NORMALS_AND_TANGENTS 0
#define DRAW_COLLISION 1


class Window
{
    public:
        Window();
        virtual ~Window();

        void run();

        GLFWwindow* getWindow(){return window;}
        float getAspect(){return float(width)/float(height);}

        int getWidth(){return width;}
        int getHeigth(){return height;}

    protected:

    private:
        bool running = 1;
        bool fullscreen = 0;
        int width = 1280;
        int height = 720;
        GLFWwindow* window;
        GLFWmonitor* monitor;
};

#endif // WINDOW_H
