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

    glViewport(0, 0, 640, 480);

    float vertices[] = {
     // Front face
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // Back face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
    };

    unsigned int indices[] ={
        0, 1, 2,  2, 3, 0,
        1, 5, 6,  6, 2, 1,
        5, 4, 7,  7, 6, 5,
        4, 0, 3,  3, 7, 4,
        3, 2, 6,  6, 7, 3,
        4, 5, 1,  1, 0, 4
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); //Ask OpenGL to create a new VAO
    glGenBuffers(1, &VBO); // ask OpenGL to create a new empty buffer (GPU memory container), VBO now refers to that buffer
    glGenBuffers(1, &EBO); // Ask OpenGL to create 1 Element Buffer Object and store its ID in EBO

    glBindVertexArray(VAO); //Make this VAO the active one

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Make this buffer the active one for vertex data. Any vertex-buffer opeation now applies to this GPU memory
    glBufferData(
        GL_ARRAY_BUFFER, //The active vertex buffer
        sizeof(vertices), //How many bytes of GPU memory to allocate
        vertices, // Copy data from this CPU array into the buffer
        GL_STATIC_DRAW //We will not change this data often
    ); 

    // Index buffer
    // Because a VAO is bound, this EBO becomes part of the VAO state
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Allocate GPU memory and upload index data into the EBO
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, // Target: index buffer
        sizeof(indices),         // Size of index data in bytes
        indices,                 // Pointer to CPU index array
        GL_STATIC_DRAW           // Hint: indices will not change
    );

    glVertexAttribPointer(
        0, //Attribute index (mathces layout location in shader)
        3, //Number of values per vertex (x, y, z)
        GL_FLOAT, //Each value is a float
        GL_FALSE, //Do not normalize the values
        3 * sizeof(float), //Number of bytes between vertices
        (void*)0 //Offset where this attribute stars in the buffer
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // The VAO now fully describes how to draw the cube

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
        glDrawElements(
            GL_TRIANGLES,          // Primitive type
            36,                    // Number of indices to read
            GL_UNSIGNED_INT,       // Type of each index
            0                      // Offset into the index buffer
        );
        
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

