#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    GLFWwindow* window;
    float r = 0.0f;

    //Initialize the library
    if(!glfwInit()){
        std::cerr << "Failed to initalize GLFW";
        return -1;
    }

    //Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if(!window){
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);


    //Loads function pointers for OpenGL
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Loop until the user closes the window
    while(!glfwWindowShouldClose(window)){
        
        r += 0.01f;
        if(r > 1.0f)
            r = 0.0f;


        glClearColor(r, 0.2f, 0.3f, 1.0f);

        //clears the window to the default color (black)
        //GL_COLOR_BUFFER_BIT means we are clearing the color buffer, what you see
        //on the screen
        glClear(GL_COLOR_BUFFER_BIT);

        //swap front and back buffers
        //Modern OpenGL uses double-buffering: you draw on the back buffer while the front
        //buffer is displayed, then swap them to avoid flickering
        glfwSwapBuffers(window);


        //Processes OS and window events, like keyboard/mouse input and window closing
        //Without htis, the window would become unresponsive or not close
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

