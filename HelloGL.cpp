#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Function definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Custom functions
void drawTriangle();

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

//Window Variables.
const unsigned int resolution_x = 1920;
const unsigned int resolution_y = 1080;

int main()
{
    
    // Initialize GLFW.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Uncomment below if on Mac.
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // Create GLFW Window object.
    GLFWwindow* window = glfwCreateWindow(resolution_x, resolution_y, "OpenGL Toy", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD system (OpenGL function pointers).
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Define OpenGL viewport.
    glViewport(0, 0, resolution_x, resolution_y);
    // Register Callback: Framebuffer
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Render Loop (frame).
    while (!glfwWindowShouldClose(window))
    {
        // Check input.
        processInput(window);
        
        // Rendering commands here.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawTriangle();

        // Check and call events and swap the buffers.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup resources.
    glfwTerminate();
    return 0;
}

// Callback function: on window resize, reset framebuffer.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Input function.
void processInput(GLFWwindow* window)
{
    //
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Draw a triangle on the canvas.
void drawTriangle()
{
    // Error checking variables.
    int  success;
    char infoLog[512];

    // Vertex data: draws a triangle.
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    // Generate and bind vertex array object.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create vertex buffer object and bind to buffer.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertex data into VBO.
    glBufferData
    (
        GL_ARRAY_BUFFER, // buffer type.
        sizeof(vertices), // size of data.
        vertices, // actual data to pass.
        GL_STATIC_DRAW // GPU draw type.
    );

    // Create vertex shader object.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Link to shader code, and compile.
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
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for compile errors.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader program.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
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

    // Set vertex attributes
    glVertexAttribPointer
    (
        0, // vertex attribute.
        3, // vertex size.
        GL_FLOAT, // data type.
        GL_FALSE, // normalize data.
        3 * sizeof(float), // size of data.
        (void*)0 //ofset (type void).
    );

    glEnableVertexAttribArray(0);

    // Draw triangle.
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}