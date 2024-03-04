#include <iostream>
#include "glad.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <chrono>
#include <thread>

bool prismOrPyramid = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        prismOrPyramid = not prismOrPyramid;
}

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

bool pyramid(GLFWwindow *window, int n)
{
    Shader myShader("./vector_shader.vs", "./fragment_shader.fs");

    // setting up vertices
    float verticesPrism[12 * n];
    float verticesPyramid[6 * (n + 1)];

    for (int i = 0; i < 2 * n; ++i)
    {
        float x = cosf(2 * M_PI * i / n) / 1.414 + (i < n ? 1 : -1) * 0.4 / 1.414;
        float y = sinf(2 * M_PI * i / n) / 1.414;
        float z = -cosf(2 * M_PI * i / n) / 1.414 + (i < n ? 1 : -1) * 0.4 / 1.414;
        verticesPrism[6 * i] = x;
        verticesPrism[6 * i + 1] = y;
        verticesPrism[6 * i + 2] = -z;
        verticesPrism[6 * i + 3] = i < n ? i < n / 2 ? i * 1.0 / n : 1 - i * 1.0 / n : 1;
        verticesPrism[6 * i + 4] = i < n ? 0 : i < 3.0 * n / 2 ? i * 1.0 / n
                                                               : 1 - i * 1.0 / n;
        verticesPrism[6 * i + 5] = i < n ? i < n / 2 ? i * 1.0 / n : 1 - i * 1.0 / n : 1;
    }

    for (int i = 0; i < n; ++i)
    {
        float x = cosf(2 * M_PI * i / n) / 1.414 + 0.4 / 1.414;
        float y = sinf(2 * M_PI * i / n) / 1.414;
        float z = -cosf(2 * M_PI * i / n) / 1.414 + 0.4 / 1.414;
        verticesPyramid[6 * i] = x;
        verticesPyramid[6 * i + 1] = y;
        verticesPyramid[6 * i + 2] = -z;
        verticesPyramid[6 * i + 3] = i < n / 2 ? i * 1.0 / n : 1 - i * 1.0 / n;
        verticesPyramid[6 * i + 4] = 0;
        verticesPyramid[6 * i + 5] = i < n / 2 ? i * 1.0 / n : 1 - i * 1.0 / n;
    }
    verticesPyramid[6 * (n)] = -0.4 / 1.414;
    verticesPyramid[6 * (n) + 1] = 0;
    verticesPyramid[6 * (n) + 2] = -0.6 / 1.414;
    verticesPyramid[6 * (n) + 3] = 1;
    verticesPyramid[6 * (n) + 4] = 1;
    verticesPyramid[6 * (n) + 5] = 1;
    // setting up indices list for each triangle, start with 0, and next 2 indices for next triangle
    unsigned int indicesPrism[6 * (n - 2) + 6 * n];
    unsigned int indicesPyramid[6 * (n - 2) + 3 * n];
    // front and back faces
    for (int i = 0; i < 2 * n - 4; ++i)
    {
        indicesPrism[3 * i] = i < n - 2 ? 0 : n;
        indicesPrism[3 * i + 1] = i < n - 2 ? (i + 1) : (i + 3);
        indicesPrism[3 * i + 2] = i < n - 2 ? (i + 2) : (i + 4);
        indicesPyramid[3 * i] = i < n - 2 ? 0 : n;
        indicesPyramid[3 * i + 1] = i < n - 2 ? (i + 1) : (i + 3);
        indicesPyramid[3 * i + 2] = i < n - 2 ? (i + 2) : (i + 4);
    }
    // sides
    for (int i = 0; i < n; ++i)
    {
        indicesPrism[3 * (2 * n - 4) + 6 * i] = i;
        indicesPrism[3 * (2 * n - 4) + 6 * i + 1] = n + i;
        indicesPrism[3 * (2 * n - 4) + 6 * i + 2] = n + (n + i + 1) % n;
        indicesPrism[3 * (2 * n - 4) + 6 * i + 3] = i;
        indicesPrism[3 * (2 * n - 4) + 6 * i + 4] = (i + 1) % n;
        indicesPrism[3 * (2 * n - 4) + 6 * i + 5] = n + (n + i + 1) % n;
        indicesPyramid[3 * (2 * n - 4) + 3 * i] = n;
        indicesPyramid[3 * (2 * n - 4) + 3 * i + 1] = i;
        indicesPyramid[3 * (2 * n - 4) + 3 * i + 2] = (i + 1) % n;
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (not glfwWindowShouldClose(window))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        processInput(window);

        glClearColor(0.5569, 0.569, 0.56, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int VBOprism, VAOprism, EBOprism;
        unsigned int VBOpyramid, VAOpyramid, EBOpyramid;

        if (prismOrPyramid)
        {
            glGenVertexArrays(1, &VAOprism);
            glGenBuffers(1, &VBOprism);
            glGenBuffers(1, &EBOprism);
            glBindVertexArray(VAOprism);

            glBindBuffer(GL_ARRAY_BUFFER, VBOprism);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPrism), verticesPrism, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOprism);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPrism), indicesPrism, GL_STATIC_DRAW);
        }
        else
        {
            glGenVertexArrays(1, &VAOpyramid);
            glGenBuffers(1, &VBOpyramid);
            glGenBuffers(1, &EBOpyramid);
            glBindVertexArray(VAOpyramid);

            glBindBuffer(GL_ARRAY_BUFFER, VBOpyramid);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPyramid), verticesPyramid, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOpyramid);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPyramid), indicesPyramid, GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        myShader.use();
        if (prismOrPyramid)
        {
            glBindVertexArray(VAOprism);
            glDrawElements(GL_TRIANGLES, 12 * n, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glBindVertexArray(VAOpyramid);
            glDrawElements(GL_TRIANGLES, 6 * (n + 1), GL_UNSIGNED_INT, 0);
        }

        glDeleteVertexArrays(1, &VAOprism);
        glDeleteBuffers(1, &VBOprism);
        glDeleteBuffers(1, &EBOprism);

        glfwSwapBuffers(window); // rendering window
        glfwPollEvents();        // updating window
    }

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
