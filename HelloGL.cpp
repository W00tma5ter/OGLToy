#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <stb_image.h>

// Function definitions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Window Size Variables.
const unsigned int resolution_x = 1920;
const unsigned int resolution_y = 1080;

//Other Misc Variables.
bool wireframe = false;

int main()
{
    // Initialize GLFW.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

    // Create GLFW Window object.
    GLFWwindow* window = glfwCreateWindow(resolution_x, resolution_y, "OpenGL Toy", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Register Callback to refresh framebuffer on window resize.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD system (OpenGL function pointers).
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Vertex and index data.
    float vertices[] = 
    {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    unsigned int indices[] = 
    {
        0, 1, 3,            // first triangle
        1, 2, 3             // second triangle
    };

    // Generate and bind vertex array object.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind vertex buffer object.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy vertex data into VBO.
    glBufferData
    (
        GL_ARRAY_BUFFER,    // buffer type.
        sizeof(vertices),   // size of data.
        vertices,           // actual data to pass.
        GL_STATIC_DRAW      // GPU draw type.
    );
    // Set vertex attributes and enable.
    // Postion attribute.
    glVertexAttribPointer
    (
        0,                  // vertex attribute.
        3,                  // vertex size.
        GL_FLOAT,           // data type.
        GL_FALSE,           // normalize data.
        6 * sizeof(float),  // size of data.
        (void*)0            // ofset (type void).
    );
    glEnableVertexAttribArray(0);
    // Color attribute.
    glVertexAttribPointer
    (
        1,                          // vertex attribute.
        3,                          // vertex size.
        GL_FLOAT,                   // data type.
        GL_FALSE,                   // normalize data.
        6 * sizeof(float),          // size of data.
        (void*)(3 * sizeof(float))  // attribute ofset (type void).
    );
    glEnableVertexAttribArray(1);

    // Create element buffer object and bind to buffers.
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Copy index data into EBO.
    glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,    // buffer type.
        sizeof(indices),            // size of data.
        indices,                    // actual data to pass.
        GL_STATIC_DRAW              // GPU draw type.
    );

    Shader ourShader("../Shaders/shader.verts", "../Shaders/shader.frags");

    // Render Loop (frame).
    while (!glfwWindowShouldClose(window))
    {
        // Check input.
        processInput(window);
        
        // Rendering commands here.
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw triangle.
            ourShader.use();

            float offset = 0.0f;
            ourShader.setFloat("xOffset", offset);

            // Draw a triangle using VBO vertices.
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Draw a rectangle using EBO indices.
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
        }

        // Check and call events and swap the buffers.
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }

    // Cleanup resources, end program.
    glfwTerminate();
    return 0;
}

// Callback function: on window resize, reset framebuffer.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Input processing.
void processInput(GLFWwindow* window)
{
    // ESC: Close application.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // SPACE: Toggle wireframe mode.
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe = !wireframe;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe = !wireframe;
        }
    }
}
