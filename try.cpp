#include <iostream>
#include <GLUT/glut.h>
#include <vector>
#include <cmath>

using namespace std;

struct Line
{
    int x_start, y_start, x_end, y_end;
};

const int SIMPLE_DDA_ALGO = 1;
const int SYMM_DDA_ALGO = 2;
const int BRESENHAM_ALGO = 3;

int selectedAlgorithm = SIMPLE_DDA_ALGO; // Default algorithm

int clickCount = 0;
int x_start, x_end, y_start, y_end;
vector<Line> lines;

float lineThickness = 1.0; // Default line thickness
float lineColorRed = 1.0;  // Default line color: red
float lineColorGreen = 0.0;
float lineColorBlue = 1.0;

void myInit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black
    glColor3f(1.0, 0.0, 1.0);         // Set lines color to magenta
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300.0, -300.0, 300.0);
}

void algorithmMenu(int value)
{
    selectedAlgorithm = value;
    glutPostRedisplay();
}

void lineThicknessMenu(int value)
{
    lineThickness = static_cast<float>(value);
    glutPostRedisplay();
}

void lineColorMenu(int value)
{
    switch (value)
    {
    case 1: // Red
        lineColorRed = 1.0;
        lineColorGreen = 0.0;
        lineColorBlue = 0.0;
        break;
    case 2: // Green
        lineColorRed = 0.0;
        lineColorGreen = 1.0;
        lineColorBlue = 0.0;
        break;
    case 3: // Blue
        lineColorRed = 0.0;
        lineColorGreen = 0.0;
        lineColorBlue = 1.0;
        break;
    }
    glutPostRedisplay();
}

void simple_dda(int x1, int y1, int x2, int y2)
{
    cout << x1 << " ," << y1 << " "
         << "||" << x2 << " ," << y2 << " " << endl;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = max(abs(dx), abs(dy));

    float x_inc = float(dx) / float(steps);
    float y_inc = float(dy) / float(steps);

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);

    for (int k = 0; k < steps; k++)
    {
        x += x_inc;
        y += y_inc;
        glVertex2i(round(x), round(y));
    }
    glEnd();
    glFlush();
}

void symm_dda(int x1, int y1, int x2, int y2)
{
    cout << x1 << " ," << y1 << " "
         << "||" << x2 << " ," << y2 << " " << endl;
    int dx, dy, step;
    float x, y, x_inc, y_inc;

    dx = x2 - x1;
    dy = y2 - y1;

    step = max(abs(dx), abs(dy));
    int n = floor(log(step) / log(2)) + 1;
    step = pow(2, n);

    x_inc = float(dx) / float(step);
    y_inc = float(dy) / float(step);

    x = x1;
    y = y1;

    glBegin(GL_POINTS);

    glVertex2i(x1, y1);
    glVertex2i(x2, y2);

    for (int k = 0; k < step; k++)
    {
        x = x + x_inc;
        y = y + y_inc;
        glVertex2i(round(x), round(y));
    }

    glEnd();
    glFlush();
}

void bresenham(int x1, int y1, int x2, int y2)
{
    cout << x1 << " ," << y1 << " "
         << "||" << x2 << " ," << y2 << " " << endl;
    int dx, dy, step, p;

    float x, y, x_inc, y_inc;
    dx = x2 - x1;
    dy = y2 - y1;
    x_inc = (dx < 0) ? -1 : 1;
    y_inc = (dy < 0) ? -1 : 1;
    x = x1;
    y = y1;
    dx = abs(dx);
    dy = abs(dy);

    glBegin(GL_POINTS);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);

    if (dy / (float)dx < 1)
    {
        p = 2 * dy - dx;
        for (int k = 0; k < abs(dx); k++)
        {
            if (p < 0)
            {
                p = p + 2 * dy;
                x = x + x_inc;
            }
            else
            {
                p = p + 2 * (dy - dx);
                x = x + x_inc;
                y = y + y_inc;
            }
            glVertex2i(round(x), round(y));
        }
    }
    else
    {
        p = 2 * dx - dy;
        for (int k = 0; k < abs(dy); k++)
        {
            if (p < 0)
            {
                p = p + 2 * dx;

                y = y + x_inc;
            }
            else
            {
                p = p + 2 * (dx - dy);
                x = x + x_inc;
                y = y + y_inc;
            }
            glVertex2i(round(x), round(y));
        }
    }
    glEnd();
    glFlush();
}

void myScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // Reset color for the background

    glBegin(GL_LINES);
    glVertex2i(300, 0);
    glVertex2i(-300, 0);
    glVertex2i(0, 300);
    glVertex2i(0, -300);
    glEnd();

    glColor3f(lineColorRed, lineColorGreen, lineColorBlue); // Set line color
    glPointSize(lineThickness);

    for (const Line &line : lines)
    {
        switch (selectedAlgorithm)
        {
        case SIMPLE_DDA_ALGO:
            simple_dda(line.x_start, line.y_start, line.x_end, line.y_end);
            break;
        case SYMM_DDA_ALGO:
            symm_dda(line.x_start, line.y_start, line.x_end, line.y_end);
            break;
        case BRESENHAM_ALGO:
            bresenham(line.x_start, line.y_start, line.x_end, line.y_end);
            break;
        }
    }

    glFlush();
}

void myMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (clickCount == 0)
        {
            x_start = x - 300;
            y_start = 300 - y;
            clickCount++;
        }
        else if (clickCount == 1)
        {
            x_end = x - 300;
            y_end = 300 - y;
            clickCount++;
            lines.push_back({x_start, y_start, x_end, y_end});
            clickCount = 0; // Reset the click count
        }
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Line Drawing Algorithms");

    myInit();

    int algorithmSubMenu = glutCreateMenu(algorithmMenu);
    glutAddMenuEntry("Simple DDA Algorithm", SIMPLE_DDA_ALGO);
    glutAddMenuEntry("Symmetric DDA Algorithm", SYMM_DDA_ALGO);
    glutAddMenuEntry("Bresenham's Algorithm", BRESENHAM_ALGO);

    int lineThicknessSubMenu = glutCreateMenu(lineThicknessMenu);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("2", 2);
    glutAddMenuEntry("3", 3);

    int lineColorSubMenu = glutCreateMenu(lineColorMenu);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);

    glutCreateMenu(algorithmMenu);
    glutAddSubMenu("Select Algorithm", algorithmSubMenu);
    glutAddSubMenu("Line Thickness", lineThicknessSubMenu);
    glutAddSubMenu("Line Color", lineColorSubMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(myScreen);
    glutMouseFunc(myMouse);

    glutMainLoop();

    return 0;
}

/*#include <iostream>
#include <GL/freeglut.h>
using namespace std;
float x = 0, y, x2, y2, m, i, j, p;
int dx = 0, dy = 0, r;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    p = 1 - r;
    while (x <= y) {
        if (p < 0) {
            x = x + 1;
            y = y;
            cout << x << " ," << y << endl;
            p = p + (2 * x) + 1;
        }
        else {
            x = x + 1;
            y = y - 1;
            cout << x << " ," << y << endl;
            p = p + (2 * x) + 1 - (2 * y);
        }
        glVertex3f(((x / 100)), ((y / 100)), 0.0);
        glVertex3f(((y / 100)), ((x / 100)), 0.0);
        glVertex3f((-(x / 100)), (-(y / 100)), 0.0);
        glVertex3f((-(x / 100)), ((y / 100)), 0.0);
        glVertex3f(((x / 100)), (-(y / 100)), 0.0);
        glVertex3f(((y / 100)), (-(x / 100)), 0.0);
        glVertex3f((-(y / 100)), (-(x / 100)), 0.0);
        glVertex3f((-(y / 100)), ((x / 100)), 0.0);
    }
    glEnd();

    glFlush();
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    cout << "Enter radius: ";
    cin >> r;
    y = r;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Drawing");
    glutDisplayFunc(display);
    init();
    glutMainLoop();

    return 0;
}
*/