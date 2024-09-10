#include "app.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "shape/box.h"
#include "renderer/texture.h"
#include "helper/app_tools.h"
#include "helper/logger.h"
#include "controller/input_controller.h"
#include "shader/shader.h"
#include "shader/shader.h"
#include "renderer/vertex_array.h"
#include "shape/pyramid.h"
#include "shape/cube.h"
#include "shape/model.h"
#include "player/player.h"
#include "renderer/renderer.h"

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Options *options = static_cast<Options *>(glfwGetWindowUserPointer(window));
    options->scroll = yoffset;
}

App::App()
{
    m_window = nullptr;
    m_options = nullptr;
}

App::~App()
{
    delete m_options;
}

Status App::setup()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        Logger::log("Error initializing GLFW\n");
        return STATUS_BAD;
    }

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                "DuelingGame", NULL, NULL);

    if (!m_window)
    {
        Logger::log("Error initializing GLFW\n");
        return STATUS_BAD;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::log("Failed to initialize GLAD\n");
        return STATUS_BAD;
    }

    // Fixes viewport issues on mac
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    m_options = new Options(); // TODO FIX, no constructor?
    glfwSetWindowUserPointer(m_window, m_options);
    glfwSetScrollCallback(m_window, scroll_callback);

    Logger::log((const char *)glGetString(GL_VERSION));
    glEnable(GL_DEPTH_TEST);

    // Enables transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    return STATUS_OK;
}

void App::fpsCounter()
{
    double curTime = glfwGetTime();
    double diff = curTime - m_prevTime;
    if (diff >= 1)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << "DuelingGame FPS(" << (m_frameCount / diff) << ")";
        glfwSetWindowTitle(m_window, stream.str().c_str());
        m_prevTime = curTime;
        m_frameCount = 0;
    }
    m_frameCount++;
}

Status App::run()
{
    // err should be STATUS_OK after setup
    Status err = setup();
    if (err == STATUS_BAD)
    {
        glfwTerminate();
        return STATUS_BAD;
    }

    Shader shader;

    err = shader.generateShader("res/Shaders/Vertex.shader",
                                "res/Shaders/Fragment.shader");
    if (err == STATUS_BAD)
    {
        glfwTerminate();
        return STATUS_BAD;
    }
    shader.useShader();

    Renderer renderer;

    Box box = Box(0.25, 0.25);
    box.translate(-0.5, 0.375, -1);
    box.setTexture("res/Images/him.PNG");
    renderer.addShape(&box, true);

    Box box2 = Box(1, 5);
    box2.setTexture("res/Images/cobble.png");
    box2.makeTiled(5, 50);
    box2.rotate(glm::vec3(glm::half_pi<float>(), 0, 0));
    renderer.addShape(&box2, true);

    Box box3 = Box(0.25, 0.25);
    box3.translate(0.5, 0.5, 0);
    renderer.addShape(&box3);

    Pyramid pyramid = Pyramid(0.05f);
    pyramid.translate(0, 2, -1);
    pyramid.setTexture("res/Images/france.png");
    renderer.addShape(&pyramid, true);

    Player player;
    renderer.addShape(player.getModel(), true);

    std::vector<Cube *> towers;
    for (int i = 0; i < 6; i++)
    {
        Cube *tower = new Cube(0.05f);
        towers.push_back(tower);
        tower->translate(1 * (i % 2) - 0.5, 0.15f, (i % 3) * -1);
        tower->scale(1, 4, 1);
        tower->setTexture("res/Images/fella.png");
        renderer.addShape(tower, true);
    }

    float r = 0;
    float i = 0.005f;

    glfwSwapInterval(1);

    // double prev = glfwGetTime();
    // float frames = 0;

    ///* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_window) && !glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
        player.move(m_window);

        pyramid.rotate(glm::vec3(0, 0.01f, 0));
        box3.setColor(r, 1.0f - r, 1.0f, 1.0f);
        r += i;
        if (r >= 1 || r <= 0)
            i *= -1;

        renderer.getCamera()->pan(m_window, m_options->scroll / 10);
        renderer.getCamera()->followModel(player.getModel()->getModelMatrix());
        m_options->scroll = 0;

        renderer.clear();

        renderer.drawShapes(&shader);

        // Swap front and back buffers, manages fps
        glfwSwapBuffers(m_window);

        // Poll for and process events
        glfwPollEvents();

        fpsCounter();
    }

    shader.deleteShader();
    glfwTerminate();
    return err;
}