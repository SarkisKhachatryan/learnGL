//
//  exercise_6_8__3.cpp
//  gl
//
//  Created by Sargis Khachatryan on 09.03.24.
//

#include "exercise_6_8__3.hpp"

#include "shader_class.hpp"

namespace exercise_6_8__3 {

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int exercise_6_8__3() {
    
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
    
    
    // use encapsulated shader class for compilation and linking of shaders
    shader_class::Shader shaderProgram("/Users/sargiskhachatryan/Desktop/project/gl/gl/vShader_pass.vsh", "/Users/sargiskhachatryan/Desktop/project/gl/gl/fShader.fsh");
    
    float vertices[] = { // as this values are passed as colors the negative ones will be clamped to 0.0f
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    
    // create and bind VAO before VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    
    // bind VAO
    glBindVertexArray(VAO);
    
    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copies vertex data(vertices) into the buffer’s(VBO) memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
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
    
    
    // for drawing only lines uncomment this line draw primitives as lines with GL_LINES
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // reder loop
    while (!glfwWindowShouldClose(window)) {
        // inputs
        processInput(window);
        
        // render
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw
        shaderProgram.use();
        glBindVertexArray(VAO);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // check events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

} // namespace exercise_6_8__3

