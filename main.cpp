#include <iostream>
#include "glad.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool prismOrPyramid = true;
float objectTranslate[3] = {0, 0, 0};
bool rotateShape = false;

glm::mat4 modelMat = glm::mat4(1.0f);
glm::mat4 viewMat = glm::mat4(1.0f);
glm::mat4 projMat = glm::mat4(1.0f);
glm::vec3 cameraLoc = {0, 0, -5.0};
glm::vec3 upVec = {0, 1, 0};
glm::vec3 rightVec = {1, 0, 0};

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

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

    // translate up (up key), down (down key), left (left key), right (right key), near (n key), far (f key)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        modelMat = glm::translate(modelMat, 0.1f * upVec);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        modelMat = glm::translate(modelMat, -0.1f * upVec);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        glm::vec3 look = glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc;
        modelMat = glm::translate(modelMat, 0.1f * glm::normalize(glm::cross(upVec, look)));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        glm::vec3 look = glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc;
        modelMat = glm::translate(modelMat, -0.1f * glm::normalize(glm::cross(upVec, look)));
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        modelMat = glm::translate(modelMat, -0.1f * look);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        modelMat = glm::translate(modelMat, 0.1f * look);
    }

    // camera move up (q key), down (e key), left (a key), right (d key), near (w key), far (s key)
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cameraLoc = cameraLoc + 0.1f * upVec;
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        upVec = glm::normalize(glm::cross(rightVec, look));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraLoc = cameraLoc - 0.1f * upVec;
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        upVec = glm::normalize(glm::cross(rightVec, look));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraLoc = cameraLoc - 0.1f * rightVec;
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        rightVec = glm::normalize(glm::cross(look, upVec));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraLoc = cameraLoc + 0.1f * rightVec;
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        rightVec = glm::normalize(glm::cross(look, upVec));
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        cameraLoc = cameraLoc + 0.1f * look;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 look = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraLoc);
        cameraLoc = cameraLoc - 0.1f * look;
    }

    // rotate about y axis
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotateShape = not rotateShape;
}

bool display(GLFWwindow *window, int n)
{
    Shader myShader("./vector_shader.vs", "./fragment_shader.fs");
    float tilt = 0;

    glEnable(GL_DEPTH_TEST);

    while (not glfwWindowShouldClose(window))
    {
        processInput(window);
        viewMat = glm::lookAt(cameraLoc, glm::vec3(0, 0, 0), upVec);
        projMat = glm::perspective(glm::radians(90.0f), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (rotateShape)
        {
            tilt += M_PI / 12;
        }

        glClearColor(0.5569, 0.569, 0.56, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        int modelMatLoc = glGetUniformLocation(myShader.id, "modelMat");
        glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
        int viewMatLoc = glGetUniformLocation(myShader.id, "viewMat");
        glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
        int projMatLoc = glGetUniformLocation(myShader.id, "projMat");
        glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

        unsigned int VBO, VAO, EBO;

        if (prismOrPyramid)
        {
            float vertices[12 * n + 12];
            for (int i = 0; i < 2 * n + 2; ++i)
            {
                if (i == 0)
                {
                    vertices[0] = 0;
                    vertices[1] = 0;
                    vertices[2] = -0.5;
                    vertices[3] = 0;
                    vertices[4] = 0;
                    vertices[5] = 0;
                    continue;
                }

                if (i == 2 * n + 1)
                {
                    vertices[12 * n + 6 + 0] = 0;
                    vertices[12 * n + 6 + 1] = 0;
                    vertices[12 * n + 6 + 2] = 0.5;
                    vertices[12 * n + 6 + 3] = 1;
                    vertices[12 * n + 6 + 4] = 1;
                    vertices[12 * n + 6 + 5] = 1;
                    continue;
                }

                else
                {
                    vertices[6 * i + 0] = 0.4 * cos(2 * M_PI * ((i - 1) % n) / n);
                    vertices[6 * i + 1] = 0.4 * sin(2 * M_PI * ((i - 1) % n) / n);
                    vertices[6 * i + 2] = i <= n ? -0.5 : 0.5;
                    vertices[6 * i + 3] = (1 + i % n) * 1.0 / (n + 1);
                    vertices[6 * i + 4] = 0;
                    vertices[6 * i + 5] = (1 + i % n) * 1.0 / (n + 1);
                }
            }

            unsigned int indices[6 * n + 6 * n];
            for (int i = 0; i < 2 * n; ++i)
            {
                indices[3 * i + 0] = i < n ? (i + 0) % n + 1 : n + (i + 0) % n + 1;
                indices[3 * i + 1] = i < n ? (i + 1) % n + 1 : n + (i + 1) % n + 1;
                indices[3 * i + 2] = i < n ? 0 : 2 * n + 1;
            }
            for (int i = 1; i <= n; ++i)
            {
                indices[6 * n + 6 * (i - 1) + 0] = i;
                indices[6 * n + 6 * (i - 1) + 1] = i + n;
                indices[6 * n + 6 * (i - 1) + 2] = n + 1 + i % n;
                indices[6 * n + 6 * (i - 1) + 3] = i;
                indices[6 * n + 6 * (i - 1) + 4] = 1 + i % n;
                indices[6 * n + 6 * (i - 1) + 5] = n + 1 + i % n;
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }
        else
        {
            float vertices[6 * n + 12];
            for (int i = 0; i < n + 2; ++i)
            {
                if (i == 0)
                {
                    vertices[0] = 0;
                    vertices[1] = 0;
                    vertices[2] = -0.5;
                    vertices[3] = 0;
                    vertices[4] = 0;
                    vertices[5] = 0;
                    continue;
                }

                if (i == n + 1)
                {
                    vertices[6 * n + 6 + 0] = 0;
                    vertices[6 * n + 6 + 1] = 0;
                    vertices[6 * n + 6 + 2] = 0.5;
                    vertices[6 * n + 6 + 3] = 1;
                    vertices[6 * n + 6 + 4] = 1;
                    vertices[6 * n + 6 + 5] = 1;
                    continue;
                }

                else
                {
                    vertices[6 * i + 0] = 0.4 * cos(2 * M_PI * ((i - 1) % n) / n);
                    vertices[6 * i + 1] = 0.4 * sin(2 * M_PI * ((i - 1) % n) / n);
                    vertices[6 * i + 2] = -0.5;
                    vertices[6 * i + 3] = (1 + i % n) * 1.0 / (n + 1);
                    vertices[6 * i + 4] = 0;
                    vertices[6 * i + 5] = (1 + i % n) * 1.0 / (n + 1);
                }
            }

            unsigned int indices[3 * n + 3 * n];
            for (int i = 0; i < n; ++i)
            {
                indices[3 * i + 0] = (i + 0) % n + 1;
                indices[3 * i + 1] = (i + 1) % n + 1;
                indices[3 * i + 2] = 0;
            }
            for (int i = 0; i < n; ++i)
            {
                indices[3 * n + 3 * i + 0] = n + 1;
                indices[3 * n + 3 * i + 1] = 1 + (i + 1) % n;
                indices[3 * n + 3 * i + 2] = i + 1;
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 48 * n, GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

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

    if (not display(window, atoi(argv[1])))
    {
        return -1;
    }

    return 0;
}
