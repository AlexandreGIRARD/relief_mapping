#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <cmath>
#include <filesystem>

#include <program.hh>
#include <camera.hh>
#include <model.hh>

using namespace glm;
void renderQuad();

uint load_texture(const char *path)
{
    uint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Texture parameter -> Bilinear and repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nbChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nbChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return tex;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


GLFWwindow *init_window(uint width, uint height)
{
    GLFWwindow *window = glfwCreateWindow(width, height, "ZIZI", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    return window;
}

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    uint width = 1920;
    uint height = 1080;
    auto window = init_window(width, height);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        exit(-1);
    }

    // Tell OpenGL window size
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Enable z-buffer computation
    glEnable(GL_DEPTH_TEST);


    // Init program and shaders
    auto shaders = program();
    shaders.add_shader("vertex.glsl", GL_VERTEX_SHADER);
    shaders.add_shader("fragment.glsl", GL_FRAGMENT_SHADER);
    shaders.link();



    // Set shader
    shaders.use();

    shaders.addUniformVec3(vec3(0, 1.0, -1), "lightPos");
    shaders.addUniformVec3(vec3(1.0, 1.0, 1.0), "lightIntensity");


    auto teapot = Model("textures/wall/teapot.obj");

    // Set camera
    Camera cam = Camera(vec3(0, 0, -2.0), vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 view = cam.look_at();
    shaders.addUniformVec3(cam.get_position(), "viewPos");
    shaders.addUniformMat4(view, "view");

    // Transform
    mat4 projection = mat4(1.0);
    projection = perspective(radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
    shaders.addUniformMat4(projection, "projection");

    mat4 model = mat4(1.0);
    model = translate(model, vec3(0,-1,1.5));
    model = rotate(model, glm::radians(-45.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    // model = rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
    // model = rotate(model, glm::radians(-90.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    shaders.addUniformMat4(model, "model");

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaders.use();

        model = rotate(model, glm::radians((float)glfwGetTime() / (float)glfwGetTime() * 0.5f), glm::normalize(glm::vec3(0.0, 1.0, 0.0))); // rotate the quad to show normal mapping from multiple directions
        shaders.addUniformMat4(model, "model");

        // Activate shader
        teapot.draw(shaders);

        // Check and call events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
