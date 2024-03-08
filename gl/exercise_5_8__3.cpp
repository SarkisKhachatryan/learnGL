//
//  exercise_5_8__3.cpp
//  gl
//
//  Created by Sargis Khachatryan on 08.03.24.
//


// Create two shader programs where the second program uses a different fragment shader
// that outputs the color yellow; draw both triangles again where one outputs the color yellow.

#include "exercise_5_8__3.hpp"

namespace exercise_5_8__3 {

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char *fragmentShaderSource1 = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

const char *fragmentShaderSource2 = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.3f, 0.2f, 1.0f, 1.0f);
}
)";

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int exercise_5_8__3() {
    glfwInit();
    
    // version setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //init window
    GLFWwindow* window = glfwCreateWindow(1200, 800, "window", NULL, NULL);
    if(window == NULL) {
        std::cout << "failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    
    
    
    // initialize, compile and check vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // success and log vars
    int success;
    char infoLog[512];
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
        << infoLog
        << std::endl;
    }
    
    
    // initialize, compile and check fragment shader
    unsigned int fragmentShader1;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);
    
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
        << infoLog
        << std::endl;
    }
    
    
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
        << infoLog
        << std::endl;
    }
    
    
    // create shader program an link shaders to it and use it
    // in this case to be able to generate different colors
    unsigned int shaderProgram1;
    shaderProgram1 = glCreateProgram();
    
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    
    glLinkProgram(shaderProgram1);
    
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
        << infoLog
        << std::endl;
    }
    
    
    // second shader program
    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();

    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    
    glLinkProgram(shaderProgram2);

    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
        << infoLog
        << std::endl;
    }
    
    
    // delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);

    
    
    float vertices1[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
    };
    
    float vertices2[] = {
        -0.5f, 0.5f, 0.0f, // top left
        -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, 0.5f, 0.0f // top right
    };
    
    // create and bind VAO before VBO, EBO
    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    
    
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    
    
    // bind VAO1
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    // copies vertex data(vertices) into the buffer’s(VBO) memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    
    // set the vertex attributes pointers
    // 1) 0 as we set in shader layout (location = 0)
    // 2) components per generic vertex attribute
    // 3) type
    // 4) normalize or not flag
    // 5) stride (consecutive vertex attribute space)
    // 6) offset of data(where it begins in buffer)
    // @note last VBO bound to GL_ARRAY_BUFFER is used by function
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 0 is vertex attribute location (index)
    glEnableVertexAttribArray(0);
    
    
    
    glBindVertexArray(VAO2);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    // copies vertex data(vertices) into the buffer’s(VBO) memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    
    
    // set the vertex attributes pointers
    // 1) 0 as we set in shader layout (location = 0)
    // 2) components per generic vertex attribute
    // 3) type
    // 4) normalize or not flag
    // 5) stride (consecutive vertex attribute space)
    // 6) offset of data(where it begins in buffer)
    // @note last VBO bound to GL_ARRAY_BUFFER is used by function
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 0 is vertex attribute location (index)
    glEnableVertexAttribArray(0);
    
    
    // now we can unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    // reder loop
    while (!glfwWindowShouldClose(window)) {
        // inputs
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw
        // use first shader program
        glUseProgram(shaderProgram1);
        
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // use second shader program
        glUseProgram(shaderProgram2);
        
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
        // check events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

} // namespace exercise_5_8__3
