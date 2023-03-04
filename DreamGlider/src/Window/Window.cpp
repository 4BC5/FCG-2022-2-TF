#include "window/Window.h"

Window::Window()
{
    glfwWindowHint(GLFW_SAMPLES, 4);//4x msaa
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//Window is not resizeable
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//Use OpenGL 3.3

    if (DEBUG)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//Use compatibility OpenGL
    else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Use modern OpenGL


    if (!glfwInit())//Initialize glfw
    {
        glfwTerminate();
        std::cout << "Failure to initialize GLFW!\n";
        exit(-1);
    }
    window = glfwCreateWindow(width,height,"Dream Glider", NULL, NULL);//Create GLFW window using width and height, name it Dream Glider
    monitor = glfwGetPrimaryMonitor();//Set the monitor the window uses
    running = true;//If the window is running
    fullscreen = false;//If the window is fullscreen
    //glClearColor(0.0f,0.0f,0.0f,1.0f);
    if (window == NULL)
    {
        glfwTerminate();
        std::cout << "Failure to create window!\n";//Throw error if window failed to initialize
        exit(-1);
    }
    glfwMakeContextCurrent(window);//Tell glfw to use this window to render
}

Window::~Window()
{
    glfwDestroyWindow(window);//Destroy current window
}

void Window::run()
{

}
