//
//  GLFWwrapper.hpp
//  lsystem
//
//  Created by Jacob Guenther on 4/3/2019
//

#ifndef GLFW_WRAPPER_HPP
#define GLFW_WRAPPER_HPP

#include "Cmd.h"
#include "Context.h"
#include "Lsystem.h"
#include "WindowCreateData.hpp"

#include <algorithm>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

class GLFWwrapper {
public:
    using LsystemContainer = std::vector<Lsystem>;
    using SystemIterator = std::vector<Lsystem>::iterator;

    explicit GLFWwrapper(WindowCreateData createData);
    GLFWwrapper(GLFWwrapper&) = delete;
    GLFWwrapper(GLFWwrapper&&) = delete;
    GLFWwrapper& operator=(const GLFWwrapper&) = delete;
    GLFWwrapper& operator=(GLFWwrapper&&) = delete;
    ~GLFWwrapper();

    void run() const;
private:
    WindowCreateData _createData;
    GLFWwindow* _window;
    int _level;
    double _scaleAmount;
    double _moveSize;

    GLdouble _tx;
    GLdouble _ty;
    GLdouble _sc;

    LsystemContainer _systems;
    SystemIterator _curfractal{_systems.end()};

    double threshold{0.003};
    static const double THRESHOLD_MAX;
    static const double THRESHOLD_MIN;

    double p1{0.0};

    std::pair<int, int> get_framebufferSize() const;
    int get_level() const;
    void adjustLevel(int level);
    void readSystemConigFile();

    void setWindowHints();
    void setInitialGLState();

    static void glfwErrorCallback(int error, const char* description);

    static const double MAX_PRESS_SPEED;
    static double LAST_PRESS_TIME;
    static void glfwKeyCallback(
        GLFWwindow *window,
        int key, int scancode,
        int action, int mods);
    static void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);

    static bool isKeyNumber(int keyMacro);
    static int numberOfKey(int keyMacro);

    static void setOrthographicMatrix(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
};

#endif

