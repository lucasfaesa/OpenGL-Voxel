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

    float vertices[] = {
    // positions
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    //Creating Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO); // ask OpenGL to create a new empty buffer (GPU memory container), VBO now refers to that buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Make this buffer the active one for vertex data. Any vertex-buffer opeation now applies to this GPU memory
    glBufferData(
        GL_ARRAY_BUFFER, //The active vertex buffer
        sizeof(vertices), //How many bytes of GPU memory to allocate
        vertices, // Copy data from this CPU array into the buffer
        GL_STATIC_DRAW //We will not change this data often
    ); 


    //Creating a Vertex array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //Ask OpenGL to create a new VAO
    glBindVertexArray(VAO); //Make this VAO the active one

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind the buffer that contains the raw vertex data

    glVertexAttribPointer(
        0, //Attribute index (mathces layout location in shader)
        3, //Number of values per vertex (x, y, z)
        GL_FLOAT, //Each value is a float
        GL_FALSE, //Do not normalize the values
        3 * sizeof(float), //Number of bytes between vertices
        (void*)0 //Offset where this attribute stars in the buffer
    );

    glEnableVertexAttribArray(0);


    //Vertex shader
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

    //Fragment shader
    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
    )";

    //Compile and link shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //container for the shader code
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); //Put the vertex shader text (the GLSL code) into the shader container
    glCompileShader(vertexShader); //Turn the shader text into something the GPU can understand

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Create a empty fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr); //Put the fragment shader into the shader container
    glCompileShader(fragmentShader); //Compile the fragment shader code

    unsigned int shaderProgram = glCreateProgram(); //Create something that can combine multiple shaders and be used when drawing
    glAttachShader(shaderProgram, vertexShader); //Add the vertex shader to the program
    glAttachShader(shaderProgram, fragmentShader);// Add the fragment shader to the program
    glLinkProgram(shaderProgram); //Connect the shaders togethr into one usable program

    glDeleteShader(vertexShader); //Delete the standalone vertex shader object, the program keeps its compiled version
    glDeleteShader(fragmentShader); //Delete the standlaone fragment shader object

    //Loop until the user closes the window
    while(!glfwWindowShouldClose(window)){
        
        //clears the window to the default color (black)
        //GL_COLOR_BUFFER_BIT means we are clearing the color buffer, what you see
        //on the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram); //Tell OpenGL: "use this shader program when drawing". This decides how vertices are processed and what color pixels get
        glBindVertexArray(VAO); //Tell OpenGL: "use these rules to read vertex data". This connects the shader to the vertex data layout
        glDrawArrays(GL_TRIANGLES, 0 , 3); //Actually draw something, Read 3 vertices, starting from the first one, Every 3 vertices form one triangle
        
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

