#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>


class Window
{
    public:
        Window();
        virtual ~Window();

        bool running = 1;
        bool fullscreen = 0;
        int width = 1280;
        int height = 720;

        void run();

        GLFWwindow* getWindow(){return window;}
        float getAspect(){return width/height;}

    protected:

    private:
    GLFWwindow* window;
    GLFWmonitor* monitor;
};

#endif // WINDOW_H
