//
//  GLFWwrapper.cpp
//  GLFWwrapper.cpp
//
//  Created by Jacob Guenther on 4/3/2019
//

#include "GLFWwrapper.hpp"

using std::pair;
using std::cout;
using std::endl;
using std::move;
using std::to_string;
using std::runtime_error;
using std::find_if;

const double GLFWwrapper::MAX_PRESS_SPEED{0.2};
double GLFWwrapper::LAST_PRESS_TIME{0};

const double GLFWwrapper::THRESHOLD_MAX{1.0};
const double GLFWwrapper::THRESHOLD_MIN{0.0001};

GLFWwrapper::GLFWwrapper(WindowCreateData createData)
    : _createData{createData}
    , _window{nullptr}
    , _level{1}
    , _scaleAmount{1.4}
    , _moveSize{0.01}
    , _tx{-0.5}, _ty{0.0}, _sc{1.0}
{
    glfwSetErrorCallback(GLFWwrapper::glfwErrorCallback);
    auto isInit = glfwInit();
    if (!isInit) {
        throw(runtime_error("Failed to init glfw"));
    }

    setWindowHints();
    _window = glfwCreateWindow(
        _createData._windowDimensions.first,
        _createData._windowDimensions.second,
        _createData._windowTitle.c_str(),
        nullptr, nullptr);
    if (_window == nullptr) {
        throw(runtime_error("Failed to create glfwCreateWindow"));
    }
    
    glfwSetWindowUserPointer(_window, this);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, 1);

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(static_cast<int>(_createData._shouldSyncFrame));
    
    setInitialGLState();

    glfwSetKeyCallback(_window, GLFWwrapper::glfwKeyCallback);
    glfwSetFramebufferSizeCallback(_window, GLFWwrapper::glfwFramebufferSizeCallback);

    readTheConfigFile();
}
GLFWwrapper::~GLFWwrapper()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}
void GLFWwrapper::glfwErrorCallback(int error, const char* description)
{
    cout << "Error Number: " << error << "::" << description << endl;
}
void GLFWwrapper::glfwKeyCallback(
    GLFWwindow *window,
    int key, int /* scancode */, int /* action */, int /* mods */)
{
    auto currentTime = glfwGetTime();
    if (currentTime - GLFWwrapper::LAST_PRESS_TIME > GLFWwrapper::MAX_PRESS_SPEED) {
        LAST_PRESS_TIME = currentTime;
    } else {
        return;
    }

    auto associatedWrapper = static_cast<GLFWwrapper*>(glfwGetWindowUserPointer(window));
    auto maybeNumber = GLFWwrapper::glfwKeyMacroToNumber(key);
    if (maybeNumber.value_or(-1) >= 0) {
        associatedWrapper->adjustLevel(maybeNumber.value());
    }
    else
    {
        auto currentLevel = associatedWrapper->get_level();
        switch (key) {
            case GLFW_KEY_ESCAPE:
                exit(0);
            case GLFW_KEY_EQUAL:
                glTranslated(-associatedWrapper->_tx, -associatedWrapper->_ty, 0);
                glScaled(associatedWrapper->_scaleAmount, associatedWrapper->_scaleAmount, 1.0);
                associatedWrapper->_sc *= associatedWrapper->_scaleAmount;
                glTranslated(associatedWrapper->_tx, associatedWrapper->_ty, 0);
                break;
            case GLFW_KEY_MINUS:
                glTranslated(-associatedWrapper->_tx, -associatedWrapper->_ty, 0);
                glScaled(1.0 / associatedWrapper->_scaleAmount, 1.0 / associatedWrapper->_scaleAmount, 1.0);
                associatedWrapper->_sc /= associatedWrapper->_scaleAmount;
                glTranslated(associatedWrapper->_tx, associatedWrapper->_ty, 0);
                break;
            case GLFW_KEY_L:
                glTranslated(associatedWrapper->_moveSize / associatedWrapper->_sc, 0, 0);
                associatedWrapper->_tx += associatedWrapper->_moveSize / associatedWrapper->_sc;
                break;
            case GLFW_KEY_J:
                glTranslated(-associatedWrapper->_moveSize / associatedWrapper->_sc, 0, 0);
                associatedWrapper->_tx -= associatedWrapper->_moveSize / associatedWrapper->_sc;
                break;
            case GLFW_KEY_I:
                glTranslated(0, associatedWrapper->_moveSize / associatedWrapper->_sc, 0);
                associatedWrapper->_ty += associatedWrapper->_moveSize / associatedWrapper->_sc;
                break;
            case GLFW_KEY_K:
                glTranslated(0, -associatedWrapper->_moveSize / associatedWrapper->_sc, 0);
                associatedWrapper->_ty -= associatedWrapper->_moveSize / associatedWrapper->_sc;
                break;
            case GLFW_KEY_COMMA:
                associatedWrapper->adjustLevel((currentLevel == 0) ? 0 : currentLevel - 1);
                break;
            case GLFW_KEY_PERIOD:
                associatedWrapper->adjustLevel(currentLevel + 1);
                break;
            case GLFW_KEY_U:
                associatedWrapper->p1 += 0.005;
                if (associatedWrapper->p1 > 1.0) {
                    associatedWrapper->p1 = 1.0;
                }
                break;
            case GLFW_KEY_Y:
                associatedWrapper->p1 -= 0.005;
                if (associatedWrapper->p1 < 0.0) {
                    associatedWrapper->p1 = 0;
                }
                break;
            case GLFW_KEY_LEFT_BRACKET:
                associatedWrapper->threshold *= 1.5;
                if (associatedWrapper->threshold > GLFWwrapper::THRESHOLD_MAX) {
                    associatedWrapper->threshold = GLFWwrapper::THRESHOLD_MAX;
                }
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                associatedWrapper->threshold *= 1.5;
                if (associatedWrapper->threshold < GLFWwrapper::THRESHOLD_MIN) {
                    associatedWrapper->threshold = GLFWwrapper::THRESHOLD_MIN;
                }
                break;
            case GLFW_KEY_BACKSLASH:
                associatedWrapper->threshold = GLFWwrapper::THRESHOLD_MIN;
                associatedWrapper->adjustLevel(200);
                break;
            default:
                break;
        }
    }
}
void GLFWwrapper::glfwFramebufferSizeCallback(GLFWwindow * /* window */, int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (width <= height) {
        setOrthographicMatrix(-0.5, 1.5, -1.0 * height / width, 1.0 * height / width);
    } else {
        setOrthographicMatrix(-0.5 * width / height, 1.5 * width / height, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void GLFWwrapper::readTheConfigFile()
{
    _systems = readlsystemfile();
    _curfractal = find_if(_systems.begin(),_systems.end(),[](const auto &f){return f.isactive();});
    if (_curfractal==_systems.end()) {
        throw(runtime_error("No active fractals in config file"));
    }
    updateTitle();
}

void GLFWwrapper::run() const
{
    while (!glfwWindowShouldClose(_window))
    {   
        Consttype vars;
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT);
        vars["p1"] = p1;

        Context context(vars, _curfractal->getExpressions());
        _curfractal->evaluateExpressions(context);
        auto drawStrategyPtr = drawStrategyFactory(_curfractal->getDrawStrategyToken());
        drawStrategyPtr->evaluateExpressions(context);
        _curfractal->draw(_level,*drawStrategyPtr);

        while (auto jj = glGetError()) {
            std::cerr << gluErrorString(jj) << endl;
        }

        glfwPollEvents();
        glfwSwapBuffers(_window);
    }
}
pair<int, int> GLFWwrapper::get_framebufferSize() const
{
    int width{0};
    int height{0};
    glfwGetFramebufferSize(_window, &width, &height);
    return {width, height};
}
int GLFWwrapper::get_level() const
{
    return _level;
}
void GLFWwrapper::adjustLevel(int level)
{
    _level = level;
    updateTitle();
}
void GLFWwrapper::updateTitle()
{
    glfwSetWindowTitle(
        _window,
        (_createData._windowTitle
        + " " + _curfractal->getname()
        + " Level: " + to_string(_level)).c_str());
}
std::optional<int> GLFWwrapper::glfwKeyMacroToNumber(int keyMacro)
{
    switch (keyMacro) {
        case GLFW_KEY_0:
            return 0;
        case GLFW_KEY_1:
            return 1;
        case GLFW_KEY_2:
            return 2;
        case GLFW_KEY_3:
            return 3;
        case GLFW_KEY_4:
            return 4;
        case GLFW_KEY_5:
            return 5;
        case GLFW_KEY_6:
            return 6;
        case GLFW_KEY_7:
            return 7;
        case GLFW_KEY_8:
            return 8;
        case GLFW_KEY_9:
            return 9;
        default:
            return {};
    };   
}

void GLFWwrapper::setOrthographicMatrix(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
#ifdef __APPLE__
    GLKMatrix4 newOrtho = GLKMatrix4MakeOrtho(float(left), float(right), float(bottom), float(top), -1.0f, 1.0f);
    glLoadMatrixf(newOrtho.m);
#else
    gluOrtho2D(left, right, bottom, top);
#endif
}

void GLFWwrapper::setWindowHints()
{
    // Window related hints
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

    // Framebuffer related hints
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    glfwWindowHint(GLFW_ACCUM_RED_BITS, 0);
    glfwWindowHint(GLFW_ACCUM_GREEN_BITS, 0);
    glfwWindowHint(GLFW_ACCUM_BLUE_BITS, 0);
    glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 0);

    glfwWindowHint(GLFW_AUX_BUFFERS, 0);

    glfwWindowHint(GLFW_STEREO, GLFW_FALSE); // hard constraint
    glfwWindowHint(GLFW_SAMPLES, static_cast<int>(_createData._numMultisamplingSamples));
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // hard constraint

    // Monitor related hints
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // Context related hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // hard constraint
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API); // hard constraint
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE); // hard constraint
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // hard constraint
}
void GLFWwrapper::setInitialGLState()
{
    auto frameSize = get_framebufferSize();
    glViewport(0, 0, frameSize.first, frameSize.second);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setOrthographicMatrix(-0.5, 1.5, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(2.0);
    glColor3d(0.8, 0.8, 0.5);
}
