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

    out float vAO;
    out float vFaceLight;
    out vec2 vTexCoord;
    out float vDistance;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        // Calculate world position
        vec4 worldPos = model * vec4(aPos, 1.0);

        // Calculate camera-relative position
        vec4 viewPos = view * worldPos;

        // The length of viewPos is the distance from the camera
        vDistance = length(viewPos.xyz);

        gl_Position = projection * viewPos;
        vAO = aAO;
        vFaceLight = aFaceLight;
        vTexCoord = aTexCoord;
    }
)";

static const char* fragmentSrc = R"(
    #version 330 core
    out vec4 FragColor;

    in float vAO;
    in float vFaceLight;
    in vec2 vTexCoord;
    in float vDistance; 

    uniform sampler2D u_Texture;

    void main()
    {
        vec4 texColor = texture(u_Texture, vTexCoord);
        if(texColor.a < 0.1) discard;

        float softenedAO = mix(0.5, 1.0, vAO);
        vec3 finalBlockColor = texColor.rgb * softenedAO * vFaceLight;

        // FOG CALCULATION
        float fogStart = 100.0; // Distance where fog begins
        float fogEnd = 240.0;   // Distance where block is 100% hidden (tune this based on your renderDistance)

        // Calculate ratio between 0.0 and 1.0
        float fogFactor = clamp((vDistance - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

        // This must match your skybox horizon color
        vec3 fogColor = vec3(0.7, 0.85, 1.0);

        // Blend the block color with the fog color
        FragColor = vec4(mix(finalBlockColor, fogColor, fogFactor), 1.0);
    }
)";


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

Shader::Shader(const char* vertexSource, const char* fragmentSource)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
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

void Shader::SetMat4(const char* name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}