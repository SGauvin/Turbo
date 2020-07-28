#include <iostream>
#include <glad/glad.h>
// Include glad before GLFW
#include <GLFW/glfw3.h>

namespace trb
{
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    const GLuint WIDTH = 800, HEIGHT = 600;

    int foo() { return 42; }

    int loop()
    {
        glfwInit();

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        std::int32_t gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (gladStatus == 0)
        {
            std::cout << "Nein" << std::endl;
            return -1;
        }

        std::cout << glGetString(GL_VERSION) << std::endl;

        // Define the viewport dimensions
        glViewport(0, 0, WIDTH, HEIGHT);

        static constexpr float data[] = {-20.0f, -20.0f, 0.0f, 20.0f, 20.0f, -20.0f};

        std::uint32_t buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        // Game loop
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        // Terminates GLFW, clearing any resources allocated by GLFW.
        glfwTerminate();
        return 0;
    }
} // namespace trb
