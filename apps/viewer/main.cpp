#include <iostream>

// Include glad and glfw for graphics and windowing
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

int main(int argc, char *argv[]) {
    std::cout << "Hi" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // -------------------------------------
    // Create window of height and width.  Initialize input callbacks
    // -------------------------------------
    GLFWmonitor* monitor =  glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    bool mainloopActive = true;

    //Set up an alternate thread for event polling
    std::thread refresh_thread;
    int refresh = 1000 * (1.0 / 60.0);

    refresh_thread = std::thread(
        [refresh, &mainloopActive]() {
            std::chrono::milliseconds time(refresh);
            while (mainloopActive) {
                std::this_thread::sleep_for(time);
                glfwPostEmptyEvent();
            }
        }
    );

    int a = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
        //glfwPollEvents();
        
        glClearColor(1.0,0,0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        a++;
        std::cout << a << std::endl;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glfwSwapBuffers(window);
    }

    mainloopActive = false;

    //glfwSetWindowUserPointer(window, this);
    refresh_thread.join();

    glfwTerminate();

    return 0;
}