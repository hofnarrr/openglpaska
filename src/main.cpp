#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "shader.hpp"

unsigned int win_w = 640;
unsigned int win_h = 400;
const std::string SHADERS_DIR = "/home/hofnarr/projects/openglpaska/src/shaders";

int** generateXorTexture(unsigned int width, unsigned int height) {
    int** xorTexArray = 0;
    xorTexArray = new int*[height];

    for (int h = 0; h < height; h++) {
        xorTexArray[h] = new int[width];
        for (int w = 0; w > width; w++) {
            xorTexArray[h][w] = h ^ w;
        }
    }
    return xorTexArray;
}

std::string file_get_contents(std::string const &path) {
    std::ostringstream ss;
    ss << std::ifstream(path).rdbuf();
    return ss.str();
}

void setViewportSize(GLFWwindow *window, int width, int height) {
    win_w = width;
    win_h = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char *argv[]) {

    const float vertices[] = {
        // position          // color
         0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  -0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  -0.5f,  0.0f, 0.5f, 0.0f, 1.0f,
         /* 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, */
         /* 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, */
        /* -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, */
        /* -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f */
    };

    const unsigned int vtxIndex[] = { 0, 1, 2 };

    const GLuint posAttrIdx = 0;
    const GLuint colorAttrIdx = 1;

    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window object
    GLFWwindow *window = glfwCreateWindow(win_w, win_h, "paska", NULL, NULL);
    if (window == NULL) {
        std::cerr << "couldn't create GLFW window :(" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "couldn't init GLAD :(" << std::endl;
        return -1;
    }

    // initial & dynamic viewport size
    glfwSetFramebufferSizeCallback(window, setViewportSize);
    glViewport(0, 0, win_w, win_h);

    // load vertex shader code
    std::string vtxShaderFile = SHADERS_DIR + "/paska.vert";
    const std::string vtxShaderSrcStr = file_get_contents(vtxShaderFile);
    const char *vtxShaderSrc = vtxShaderSrcStr.c_str();
    std::cout << "=== vtxShader: " << vtxShaderFile << std::endl;
    std::cout << vtxShaderSrc << std::endl;
    std::cout << "=============" << std::endl << std::endl; 

    // load fragment shader code
    std::string fragShaderFile = SHADERS_DIR + "/paska.frag";
    const std::string fragShaderSrcStr = file_get_contents(fragShaderFile);
    const char *fragShaderSrc = fragShaderSrcStr.c_str();
    std::cout << "=== fragShader: " << fragShaderFile << std::endl;
    std::cout << fragShaderSrc << std::endl;
    std::cout << "=============" << std::endl << std::endl;

    // create shader
    Shader shader(vtxShaderSrc, fragShaderSrc);

    // create buffers
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind VAO, bind&set VBO and EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vtxIndex), vtxIndex,
                 GL_STATIC_DRAW);

    // set vtx attribs
    // position
    // glVertexAttribPointer(location, size, gl type, normalize?, stride, offset)
    glVertexAttribPointer(posAttrIdx, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(posAttrIdx);
    // color
    glVertexAttribPointer(colorAttrIdx, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colorAttrIdx);

    // unbind VBO & VAO, keep EBO!
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

    // rendering loop
    while (!glfwWindowShouldClose(window)) {
        // shader uniform defs
        float suf_dTime = glfwGetTime();

        processInput(window);

        // blank
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        shader.set2UInt("iResolution", win_w, win_h);
        shader.setDouble("dTime", suf_dTime);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
