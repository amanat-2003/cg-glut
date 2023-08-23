#include <iostream>
#include <GLUT/glut.h>
using namespace std;

const float windowHeight = 700.0;
const float windowWidth = 700.0;

int pntX1, pntY1, r;

void plotCenter()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2);
    glBegin(GL_POINTS);
    glVertex2i(windowWidth / 2 + pntX1, windowWidth / 2 + pntY1);
    glEnd();
    glFlush();
}

void drawQuad()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(windowWidth / 2, 0.0);
    glVertex2f(windowWidth / 2, windowHeight);
    glVertex2f(0.0, windowHeight / 2);
    glVertex2f(windowWidth, windowHeight / 2);
    glEnd();
    glFlush();
}

void plot(int x, int y)
{
    float ptX = windowWidth / 2 + x + pntX1;
    float ptY = windowHeight / 2 + (y + pntY1);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    glVertex2i(ptX, ptY);
    glEnd();
    glFlush();
}

void myInit(void)
{
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Mid Point Circle Drawing");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowWidth, 0.0, windowHeight, 1.0, -1.0);
}

void midPointCircleAlgo()
{
    int x = 0;
    int y = r;
    float decision = 5 / 4 - r;
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

void myDisplay(void)
{
    glColor3f(0.0, 0.0, 0.0);
    glFlush();
}

int main(int argc, char **argv)
{
    cout << "Enter the coordinates of the center:\n\n"
         << endl;

    cout << "Enter X-coordinate:- ";
    cin >> pntX1;
    cout << "\nEnter Y-coordinate:- ";
    cin >> pntY1;
    cout << "\nEnter Enter radius:- ";
    cin >> r;
    glutInit(&argc, argv);
    myInit();
    glutDisplayFunc(myDisplay);
    drawQuad();
    plotCenter();
    midPointCircleAlgo();

    glutMainLoop();

    return 0;
}
