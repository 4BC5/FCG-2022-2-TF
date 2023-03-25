#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#define DRAW_COLLISION
//#define DEBUG
//#define DRAW_AABB
#define DRAW_LIGHT_AABB
//#define DRAW_MESH_AABB


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

        static void window_size_callback(GLFWwindow* window, int width, int height);

    protected:
        void onResize(int width, int height);
    private:
        bool running = 1;
        bool fullscreen = 0;
        int width = 1280;
        int height = 720;
        GLFWwindow* window;
        GLFWmonitor* monitor;
};

#endif // WINDOW_H
