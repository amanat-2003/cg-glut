//
//  main.cpp
//  full cg project
//
//  Created by Amanat Singh on 23/08/23.
//

#include <iostream>
#include <GLUT/glut.h>
#include <vector>
#include <cmath>
using namespace std;

struct Line
{
    int x_start, y_start, x_end, y_end;
};

const int windowHeight = 600.0;
const int windowWidth = 600.0;

const int LINE = 1;
const int CIRCLE = 2;

const int SIMPLE_DDA_ALGO = 1;
const int SYMM_DDA_ALGO = 2;
const int BRESENHAM_ALGO = 3;

// Define default shape, algorithm, thickness, color for the drawn lines
int defaultDrawShape = LINE; // Default Shape to draw

int selectedAlgorithm = SIMPLE_DDA_ALGO; // Default algorithm

float lineThickness = 1.0; // Default line thickness

float lineColorRed = 0.0; // Default line color: green
float lineColorGreen = 1.0;
float lineColorBlue = 0.0;

int clickCount = 0;
int x_start, x_end, y_start, y_end;

int center_x = 0, center_y = 0, radius = 0;

vector<Line> lines;

void myInit()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Full CG Project");

    glClearColor(0.3242, 0.1289, 0.4882, 0.2); // Set background color to purple
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2);
}

void shapeMenu(int value)
{
    if (defaultDrawShape != value)
    {
        lines.clear();
        center_x = 0;
        center_y = 0;
        radius = 0;
        clickCount = 0;
    }
    defaultDrawShape = value;

    glutPostRedisplay();
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
    case 1: // Violet
        lineColorRed = 0.0;
        lineColorGreen = 0.0;
        lineColorBlue = 0.5;
        break;
    case 2: // Blue
        lineColorRed = 0.0;
        lineColorGreen = 0.0;
        lineColorBlue = 1.0;
        break;
    case 3: // Green
        lineColorRed = 0.0;
        lineColorGreen = 1.0;
        lineColorBlue = 0.0;
        break;
    case 4: // Yellow
        lineColorRed = 1.0;
        lineColorGreen = 1.0;
        lineColorBlue = 0.0;
        break;
    case 5: // Orange
        lineColorRed = 1.0;
        lineColorGreen = 0.6;
        lineColorBlue = 0.0;
        break;
    case 6: // Red
        lineColorRed = 1.0;
        lineColorGreen = 0.0;
        lineColorBlue = 0.0;
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

    for (int k = 0; k < steps; k++)
    {
        x += x_inc;
        y += y_inc;
        glVertex2i(round(x), round(y));
    }

    glVertex2i(x2, y2);
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

void drawAxes()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 0.1); // Reset color for the axes

    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(300, 0);
    glVertex2i(-300, 0);
    glVertex2i(0, 300);
    glVertex2i(0, -300);
    glEnd();
}

void plotCenter()
{
    glPointSize(lineThickness + 2);
    glBegin(GL_POINTS);
    glVertex2i(center_x, center_y);
    glEnd();
    glFlush();
}

void plot(int x, int y)
{
    float ptX = x + center_x;
    float ptY = (y + center_y);
    glBegin(GL_POINTS);
    glVertex2i(round(ptX), round(ptY));
    glEnd();
    glFlush();
}

void midPointCircleAlgo()
{
    int x = 0;
    int y = radius;
    float decision = 5 / 4 - radius;
    plot(x, y);

    while (y > x)
    {
        if (decision < 0)
        {
            x++;
            decision += 2 * x + 1;
        }
        else
        {
            y--;
            x++;
            decision += 2 * (x - y) + 1;
        }
        plot(x, y);
        plot(x, -y);
        plot(-x, y);
        plot(-x, -y);
        plot(y, x);
        plot(-y, x);
        plot(y, -x);
        plot(-y, -x);
    }
}

void myScreen()
{
    drawAxes();

    glColor3f(lineColorRed, lineColorGreen, lineColorBlue); // Set line color
    glPointSize(lineThickness);

    switch (defaultDrawShape)
    {
    case LINE:
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
        break;

    case CIRCLE:
        plotCenter();
        midPointCircleAlgo();
        break;

    default:
        break;
    }

    glFlush();
}

void myMouse(int button, int state, int x, int y)
{
    // cout << "x = " << x << endl;
    // cout << "y = " << y << endl;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (clickCount == 0)
        {
            x_start = x - 300;
            y_start = 300 - y;
            if (defaultDrawShape == CIRCLE)
            {
                center_x = x_start;
                center_y = y_start;
                radius = 0;
                cout << "center_x = " << center_x << endl;
                cout << "center_y = " << center_y << endl;
                cout << "radius = " << radius << endl;
            }
            clickCount++;
        }
        else if (clickCount == 1)
        {
            x_end = x - 300;
            y_end = 300 - y;
            if (defaultDrawShape == CIRCLE)
            {
                radius = int(sqrt(pow((x_end - x_start), 2) + pow((y_end - y_start), 2)));
                cout << "radius = " << radius << endl;
            }
            else
            {
                lines.push_back({x_start, y_start, x_end, y_end});
            }
            // clickCount++;
            clickCount = 0; // Reset the click count
        }
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    myInit();

    int shapeSelectMenu = glutCreateMenu(shapeMenu);
    glutAddMenuEntry("Line", LINE);
    glutAddMenuEntry("Circle", CIRCLE);

    int algorithmSubMenu = glutCreateMenu(algorithmMenu);
    glutAddMenuEntry("Simple DDA Algorithm", SIMPLE_DDA_ALGO);
    glutAddMenuEntry("Symmetric DDA Algorithm", SYMM_DDA_ALGO);
    glutAddMenuEntry("Bresenham's Algorithm", BRESENHAM_ALGO);

    int lineThicknessSubMenu = glutCreateMenu(lineThicknessMenu);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("2", 2);
    glutAddMenuEntry("3", 3);
    glutAddMenuEntry("4", 4);
    glutAddMenuEntry("5", 5);

    int lineColorSubMenu = glutCreateMenu(lineColorMenu);
    glutAddMenuEntry("Violet", 1);
    glutAddMenuEntry("Blue", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Yellow", 4);
    glutAddMenuEntry("Orange", 5);
    glutAddMenuEntry("Red", 6);

    glutCreateMenu(algorithmMenu);
    glutAddSubMenu("Select Shape", shapeSelectMenu);
    glutAddSubMenu("Select Algorithm", algorithmSubMenu);
    glutAddSubMenu("Line Thickness", lineThicknessSubMenu);
    glutAddSubMenu("Line Color", lineColorSubMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(myScreen);
    glutMouseFunc(myMouse);

    glutMainLoop();

    return 0;
}
