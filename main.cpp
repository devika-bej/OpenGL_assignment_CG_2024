#include <iostream>
#include "glad.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <chrono>
#include <thread>

bool prismOrPyramid = true;
float objectTranslate[3] = {0, 0, 0};
bool rotateShape = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // toggle between prism and pyramid
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        prismOrPyramid = not prismOrPyramid;

    // up (up key), down (down key), left (left key), right (right key), near (n key), far (f key)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        objectTranslate[1] += 0.1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        objectTranslate[1] -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        objectTranslate[0] -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        objectTranslate[0] += 0.1;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        objectTranslate[2] -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        objectTranslate[2] += 0.1;

    // rotate about z axis or y axis (will decide later)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotateShape = true;
}

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

bool pyramid(GLFWwindow *window, int n)
{
    Shader myShader("./vector_shader.vs", "./fragment_shader.fs");

    while (not glfwWindowShouldClose(window))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        processInput(window);

        glClearColor(0.5569, 0.569, 0.56, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();

        int rotationVecLoc = glGetUniformLocation(myShader.id, "objectTranslate");
        glUniform3f(rotationVecLoc, objectTranslate[0], objectTranslate[1], objectTranslate[2]);

        float vertices[18] = {0.5, 0.5, 0.5, 1, 1, 0, 0.5, -0.5, 0.5, 0, 0, 1, -0.5, 0, 0, 1, 0, 1};
        unsigned int indices[3] = {2, 0, 1};

        unsigned int VBOprism, VAOprism, EBOprism;

        glGenVertexArrays(1, &VAOprism);
        glGenBuffers(1, &VBOprism);
        glGenBuffers(1, &EBOprism);
        glBindVertexArray(VAOprism);

        glBindBuffer(GL_ARRAY_BUFFER, VBOprism);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOprism);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(VAOprism);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

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
