#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Function definitions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Shader code.
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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
        return -1;
    }



    // Build and compile shader program.
    // Error checking variables.
    int  success;
    char infoLog[512];
    // Create vertex shader object.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Link shader code, and compile.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile errors.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Create fragment shader object.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Link shader code, and compile.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile errors.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Create shader program object.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // Link shaders to program.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for program link errors.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Cleanup shaders after linking.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // Vertex and index data.
    float vertices[] = 
    {
     0.5f,  0.5f, 0.0f,     // top right
     0.5f, -0.5f, 0.0f,     // bottom right
    -0.5f, -0.5f, 0.0f,     // bottom left
    -0.5f,  0.5f, 0.0f      // top left 
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
    glVertexAttribPointer
    (
        0,                  // vertex attribute.
        3,                  // vertex size.
        GL_FLOAT,           // data type.
        GL_FALSE,           // normalize data.
        3 * sizeof(float),  // size of data.
        (void*)0            // ofset (type void).
    );
    glEnableVertexAttribArray(0);

    // Create element buffer object and bind to buffers.
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Copy index data into EBO.
    glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER, 
        sizeof(indices), 
        indices, 
        GL_STATIC_DRAW
    );

    

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
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            // Draw a triangle using VBO vertices.
            //glDrawArrays(GL_TRIANGLES, 0, 3);

            // Draw a rectangle using EBO indices.
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
