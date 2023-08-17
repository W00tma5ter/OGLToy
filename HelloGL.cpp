#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function definitions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Window Size Variables.
const unsigned int resolution_x = 1080;
const unsigned int resolution_y = 1080;

//Other Misc Variables.
bool wireframe = false;

int main()
{
    //System initialization code.
    //---------------------------------------------------------------------------
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

    // Initialize GLAD system (OpenGL function pointers).
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Register Callbacks.
    //---------------------------------------------------------------------------
    // Refresh framebuffer on window resize.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Generate shader from external files.
    //---------------------------------------------------------------------------
    Shader ourShader("../Shaders/shader.verts", "../Shaders/shader.frags");

    // Vertex and index data.
    //---------------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = 
    {
        0, 1, 3,            // first triangle
        1, 2, 3             // second triangle
    };

    // Generate and bind vertex array object.
    //---------------------------------------------------------------------------
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind vertex buffer object.
    //---------------------------------------------------------------------------
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy vertex data into VBO.
    glBufferData
    (
        GL_ARRAY_BUFFER,            // buffer type.
        sizeof(vertices),           // size of data.
        vertices,                   // actual data to pass.
        GL_STATIC_DRAW              // GPU draw type.
    );

    // Set vertex attributes and enable.
    //---------------------------------------------------------------------------
    // Postion attribute.
    glVertexAttribPointer
    (
        0,                          // vertex attribute.
        3,                          // vertex size.
        GL_FLOAT,                   // data type.
        GL_FALSE,                   // normalize data.
        8 * sizeof(float),          // size of data.
        (void*)0                    // attribute ofset (type void).
    );
    glEnableVertexAttribArray(0);
    // Color attribute.
    glVertexAttribPointer
    (
        1,                          // vertex attribute.
        3,                          // vertex size.
        GL_FLOAT,                   // data type.
        GL_FALSE,                   // normalize data.
        8 * sizeof(float),          // size of data.
        (void*)(3 * sizeof(float))  // attribute ofset (type void).
    );
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute.
    glVertexAttribPointer
    (
        2,                          // vertex attribute.
        2,                          // vertex size.
        GL_FLOAT,                   // data type.
        GL_FALSE,                   // normalize data.
        8 * sizeof(float),          // size of data.
        (void*)(6 * sizeof(float))  // attribute ofset (type void).
    );
    glEnableVertexAttribArray(2);

    // Create element buffer object and bind to buffers.
    //---------------------------------------------------------------------------
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

    // Generate textures from external file.
    //---------------------------------------------------------------------------
    unsigned int texture1, texture2;
    int width, height, nrChannels;
    unsigned char* data;

    // Flip textures on load.
    stbi_set_flip_vertically_on_load(true);

    // Texture 1.
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image and generate texture.
    data = stbi_load("../Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Texture 2.
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image and generate texture.
    data = stbi_load("../Textures/Mable.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);
    
    // Render Loop (frame).
    //---------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Check input.
        processInput(window);
        
        // Rendering commands here.
        {
            // Set background color.
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //Set offset.
            ourShader.use();
            float offset = 0.0f;
            ourShader.setFloat("xOffset", offset);
            
            // Bind textures to texture units.
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            // Update Transforms.
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
            trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            // Send transform to shader.
            unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


            // Draw something.
            ourShader.use();
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // Check and call events and swap the buffers.
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }
    //---------------------------------------------------------------------------
    
    // Cleanup resources, end program.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
