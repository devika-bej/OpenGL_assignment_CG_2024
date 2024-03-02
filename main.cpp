#include <iostream>
#include "glad.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <math.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

bool pyramid(GLFWwindow *window)
{
    Shader myShader("./vector_shader.vs", "./fragment_shader.fs");

    int n = 9;
    // setting up vertices
    float vertices[3 * n];
    for (int i = 0; i < n; ++i)
    {
        float x = cosf(2 * M_PI * i / n);
        float y = sinf(2 * M_PI * i / n);
        vertices[3 * i] = x;
        vertices[3 * i + 1] = y;
        vertices[3 * i + 2] = 0.0f;
    }
    // setting up indices list for each triangle, start with 0, and next 2 indices for next triangle
    unsigned int indices[3 * (n - 2)];
    for (int i = 0; i < n - 2; ++i)
    {
        indices[3 * i] = 0;
        indices[3 * i + 1] = i + 1;
        indices[3 * i + 2] = i + 2;
    }

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // index = 0, size = 3, float type, dont normalize, since each attribute is x,y,z therefore stride = 3*size of float, type of draw = static
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (not glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.5569, 0.569, 0.56, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3 * n, GL_UNSIGNED_INT, 0);
        // type of element is triangle, 3*n elements in vertices, indices is of uint type, 0 is pointer to indices

        glfwSwapBuffers(window); // rendering window
        glfwPollEvents();        // updating window
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return true;
}

int main(int argc, char **argv)
{
    // set up for opengl 3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // major version - 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // minor version - 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile

    // window creation
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Scene 1", nullptr, nullptr);
    if (not window)
    {
        std::cout << "Window could not be created\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD could not be initialized\n";
        glfwTerminate();
        return false;
    }

    if (not pyramid(window))
    {
        return -1;
    }

    return 0;
}
