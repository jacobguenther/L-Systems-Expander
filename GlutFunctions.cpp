//
//  glutFunctions.cpp
//  lsystems
//
//  Created by Chris Hartman on 4/21/18.
//

#include "Cmd.h"
#include "Context.h"
#include "Lsystem.h"
#include <algorithm>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <sstream>
#include <vector>

void readtheconfigfile();

using std::endl;
using std::find_if;
using std::runtime_error;
using std::vector;

namespace {

int main_menu_id;
GLdouble tx = -0.5, ty = 0, sc = 1;
int level = 1;
vector<Lsystem> systems;
auto curfractal=systems.end();
double p1 = 0;
double thresh = 0.003;
const double THRESHMAX = 1.0;
const double THRESHMIN = .0001;

void display() {
    Consttype vars;
    
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT);
    vars["p1"] = p1;
    Context context(vars,curfractal->getExpressions());
    curfractal->evaluateExpressions(context);
    curfractal->fixRules(context);
    auto drawStrategyPtr = drawStrategyFactory(curfractal->getDrawStrategyToken());
    drawStrategyPtr->evaluateExpressions(context);
    curfractal->draw(level,*drawStrategyPtr);
    glutSwapBuffers();
    while (auto jj = glGetError())
        std::cerr << gluErrorString(jj) << endl;
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-0.5, 1.5, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(2.0);
    glColor3d(.8, .8, .5);
}

void change_window_title() {
    std::ostringstream os1;
    os1 << "L' Systems Fractal: " << curfractal->getname()
    << " Level: " << level;
    
    glutSetWindowTitle(os1.str().c_str());
}
    
void adjust_level(int newlevel) {
    level = newlevel;
    change_window_title();
    glutPostRedisplay();
}

void handle_frac_menu(int value) {
    curfractal = systems.begin()+value;
    adjust_level(1);
    glutPostRedisplay();
}

void handle_main_menu(int value) {
    if (value < 100)
        handle_frac_menu(value);
    else if (value == 103)
        readtheconfigfile();
}

void keyboard(unsigned char dakey, int /*unused*/, int /*unused*/) {
    const GLdouble SCALEAMOUNT = 1.4;
    const double MOVESIZE = .01;
    if (dakey >= '0' && dakey <= '9')
        adjust_level(dakey - '0');
    else
        switch (dakey) {
            case 27:  //ESC
                exit(0);
            case '=':
                glTranslated(-tx, -ty, 0);
                glScaled(SCALEAMOUNT, SCALEAMOUNT, 1);
                sc *= SCALEAMOUNT;
                glTranslated(tx, ty, 0);
                glutPostRedisplay();
                break;
            case '-':
                glTranslated(-tx, -ty, 0);
                glScaled(1.0 / SCALEAMOUNT, 1.0 / SCALEAMOUNT, 1);
                sc /= SCALEAMOUNT;
                glTranslated(tx, ty, 0);
                glutPostRedisplay();
                break;
            case 'l':
                glTranslated(MOVESIZE / sc, 0, 0);
                tx += MOVESIZE / sc;
                glutPostRedisplay();
                break;
            case 'j':
                glTranslated(-MOVESIZE / sc, 0, 0);
                tx -= MOVESIZE / sc;
                glutPostRedisplay();
                break;
            case 'i':
                glTranslated(0, MOVESIZE / sc, 0);
                ty += MOVESIZE / sc;
                glutPostRedisplay();
                break;
            case 'k':
                glTranslated(0, -MOVESIZE / sc, 0);
                ty -= MOVESIZE / sc;
                glutPostRedisplay();
                break;
            case ',':
                adjust_level(level == 0 ? 0 : level - 1);
                break;
            case '.':
                adjust_level(level + 1);
                break;
            case 'u':
                p1 += .005;
                if (p1 > 1.)
                    p1 = 1.;
                glutPostRedisplay();
                break;
            case 'y':
                p1 -= .005;
                if (p1 < 0.)
                    p1 = 0;
                glutPostRedisplay();
                break;
            case '[':
                thresh *= 1.5;
                if (thresh > THRESHMAX)
                    thresh = THRESHMAX;
                glutPostRedisplay();
                break;
            case ']':
                thresh /= 1.5;
                if (thresh < THRESHMIN)
                    thresh = THRESHMIN;
                glutPostRedisplay();
                break;
            case '\\':
                thresh = THRESHMIN;
                adjust_level(200);
                glutPostRedisplay();
                break;
            case 'f':
                glutPostRedisplay();  //!!! Why?
        }
}

void reshape(int w, int h) {
    glViewport(0, 0, GLsizei(w), GLsizei(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h)
        gluOrtho2D(-0.5, 1.5, -1.0 * h / w, 1.0 * h / w);
    else
        gluOrtho2D(-0.5 * w / h, 1.5 * w / h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

}  // namespace

void readtheconfigfile() {
    systems = readlsystemfile();
    curfractal = find_if(systems.begin(),systems.end(),[](const auto &f){return f.isactive();});
    if (curfractal==systems.end())
        throw(runtime_error("No active fractals in config file"));
    change_window_title();
}

void runGlut(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    glutInitWindowSize(801, 801);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("L' Systems");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    main_menu_id = glutCreateMenu(handle_main_menu);
    glutAddMenuEntry("Reread config file", 103);
    
    for (auto ii = 0u; ii < systems.size(); ++ii)
        if (systems[ii].isactive())
            glutAddMenuEntry(systems[ii].getname().c_str(), int(ii));
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    handle_frac_menu(0);
    glutMainLoop();
}
