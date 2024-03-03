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

bool pyramid(GLFWwindow *window, int n)
{
    Shader myShader("./vector_shader.vs", "./fragment_shader.fs");

    int Total[] = {n, 0};
    // setting up vertices
    float vertices[6 * n];
    for (int i = 0; i < 2 * n; ++i)
    {
        float x = cosf(2 * M_PI * i / n) / 1.414 + (i < n ? 1 : -1) * 0.4 / 1.414;
        // float y = sinf(2 * M_PI * i / n) / 1.414 + (i < n ? 0.2 : -0.2);
        float y = sinf(2 * M_PI * i / n) / 1.414;
        float z = -cosf(2 * M_PI * i / n) / 1.414 + (i < n ? 1 : -1) * 0.4 / 1.414;
        vertices[3 * i] = x;
        vertices[3 * i + 1] = y;
        vertices[3 * i + 2] = -z;
    }
    // setting up indices list for each triangle, start with 0, and next 2 indices for next triangle
    unsigned int indices[6 * (2 * n - 2)];
    // front and back faces
    for (int i = 0; i < 2 * n - 4; ++i)
    {
        indices[3 * i] = i < n - 2 ? 0 : n;
        indices[3 * i + 1] = i < n - 2 ? (i + 1) : (i + 3);
        indices[3 * i + 2] = i < n - 2 ? (i + 2) : (i + 4);
    }
    // side rectangles
    for (int i = 0; i < n; ++i)
    {
        indices[3 * (2 * n - 4) + 6 * i] = i;
        indices[3 * (2 * n - 4) + 6 * i + 1] = n + i;
        indices[3 * (2 * n - 4) + 6 * i + 2] = n + (n + i + 1) % n;
        indices[3 * (2 * n - 4) + 6 * i + 3] = i;
        indices[3 * (2 * n - 4) + 6 * i + 4] = (i + 1) % n;
        indices[3 * (2 * n - 4) + 6 * i + 5] = n + (n + i + 1) % n;
    }
    for (auto &i : indices)
        std::cout << i << '\n';

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (not glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.5569, 0.569, 0.56, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12 * n, GL_UNSIGNED_INT, 0);
        // type of element is triangle, 12*n elements in vertices, indices is of uint type, 0 is pointer to indices

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

    if (not pyramid(window, atoi(argv[1])))
    {
        return -1;
    }

    return 0;
}
