//
// Created by lucas on 08/02/2026.
//

#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


static const char* vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in float aAO;
    layout (location = 2) in float aFaceLight;
    layout (location = 3) in vec2 aTexCoord;

    out float vAO; //passing this to the fragment shader
    out float vFaceLight;
    out vec2 vTexCoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        vAO = aAO;
        vFaceLight = aFaceLight;
        vTexCoord = aTexCoord;
    }
)";

/*
static const char* vertexSrc = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor; // New color input

    out vec3 ourColor; // Passing this to fragment shader

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";
*/

//in the fragment shader, out variables are meant to be sent to the screen
static const char* fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in float vAO; //received from the vertex shader
    in float vFaceLight;
    uniform sampler2D u_Texture;
    in vec2 vTexCoord;

    void main()
    {
        vec4 texColor = texture(u_Texture, vTexCoord);
        if(texColor.a < 0.1) discard;

        float softenedAO = mix(0.5, 1.0, vAO);
        FragColor = vec4(texColor.rgb * softenedAO * vFaceLight, 1.0);
    }
)";

/*
static const char* fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 ourColor; // Received from vertex shader

    void main()
    {
        FragColor = vec4(ourColor, 1.0);
    }
)";
*/

Shader::Shader()
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Bind() const {
    glUseProgram(ID);
}

void Shader::SetMat4(const char *name, const glm::mat4 &mat) {
    int loc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetInt(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
