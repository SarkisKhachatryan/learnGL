//
//  main_till_7_7.cpp
//  gl
//
//  Created by Sargis Khachatryan on 11.03.24.
//

#include "main_till_7_7.hpp"

#include "shader_class.hpp"

// coment or uncomment this define preprocessor whenever you want to run this file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

namespace main_till_7_7 {

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main_till_7_7() {
    
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
    shader_class::Shader shaderProgram("/Users/sargiskhachatryan/Desktop/project/gl/gl/vShader_v2.vsh", "/Users/sargiskhachatryan/Desktop/project/gl/gl/fShader_v2.fsh");
    
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    // create and bind VAO before VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // bind VAO befor others, to record their activity
    glBindVertexArray(VAO);
    
    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copies vertex data(vertices) into the buffer’s(VBO) memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // bind EOB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // copies indecis data(indeces of each vertex respecevely) into the buffer’s(EBO) memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    // set the vertex attributes pointers
    // 1) 0 as we set in shader layout (location = 0)
    // 2) components per generic vertex attribute
    // 3) type
    // 4) normalize or not flag
    // 5) stride (consecutive vertex attribute space)
    // 6) offset of data(where it begins in buffer)
    // @note last VBO bound to GL_ARRAY_BUFFER is used by function
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // 0 is vertex attribute location (index)
    glEnableVertexAttribArray(0);
    
    // set the vertex attributes(colors) pointers
    // @note last VBO bound to GL_ARRAY_BUFFER is used by function
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // 0 is vertex attribute location (index)
    glEnableVertexAttribArray(1);
    
    // set the vertex attribute(texture coords)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    
//     generate bind and configure texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    
    glActiveTexture(GL_TEXTURE0);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    // load the image
    int width, height, nChannels;
    unsigned char *imageData = stbi_load("/Users/sargiskhachatryan/Desktop/project/gl/gl/container.jpg", &width, &height, &nChannels, 0);

    if (imageData) {
        // generate texture
        // 1) specifies the texture target, only 2d-s will be affected, no 3d or 1d
        // 2) specifies the mipmap level for which we want to create a texture for
        // 3) format of pixel we want to store
        // 4-5) size of texture
        // 6) idk, what is it ????
        // 7-8) we loaded from RGB and store them to unsigend char, destination to target
        // 9) lastly the actual image we stored as unsiged char
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        // generate minimap, this call is effective after texture creation, so order is important here
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "failed to load data from memory" << std::endl;
    }
    // now we can free image data, as GL_TEXTURE_2D already holds the texture
    // even if it fails
    stbi_image_free(imageData);
    
    
    
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
        
        // render the triangles from an index buffer.
        // 1) mode to draw (triangles)
        // 2) 6 vertices in total to draw
        // 3) type of indeces
        // 4) offset of buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

} // namespace main_till_7_7
