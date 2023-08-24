#include <GLUT/glut.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
using namespace std;

void init(void)
{
    glutInitWindowSize(500,500);
    glutInitWindowPosition(1000,130);
    glutCreateWindow("Mid Point Ellipse Drawing");
    glClearColor(0.53,0.81,0.92,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-250,250,-250,250,1,-1);
}
void displayAxis(void)
{
     glPointSize(1);
     glColor3f(1.0,1.0,1.0);
     glBegin(GL_LINES);
     glVertex2i(0,250);
     glVertex2i(0,-250);
     glVertex2i(250,0);
     glVertex2i(-250,0);
     glEnd();
     glFlush();
}
void plotPoints(int x1, int y1, int x, int y)
{
     float xf = x1+x;
     float yf = y1+y;
     glColor3f(0.0,0.0,1.0);
     glPointSize(2);
     glBegin(GL_POINTS);

     glVertex2i(xf-250, 250-yf);
     glEnd();
     glFlush();
}
void ellipse(int x1, int y1, int x2, int y2, int x3, int y3)
{
    long int ry = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
    long int rx = sqrt(pow((x1 - x3), 2) + pow((y1 - y3), 2));
    bool flag = 1;
    int x = 0;
    int y = ry;
    plotPoints(x1, y1, x, y);

    float condition = (2 * ry * ry * x) - (2 * rx * rx * y);
    float pk = (ry * ry) - (ry * rx * rx) + (pow(rx,2) * 0.25);
    while (y != 0) {
        if (condition < 0)
        {
            if (pk >= 0)
            {
                x++;
                y--;
                pk += (2 * ry * ry * x) + ry * ry - (2 * rx * rx * y);
            }
            else {
                x++;
                pk += ry * ry * 2 * x + ry * ry;
            }
            condition = (2 * ry * ry * x) - (2 * rx * rx * y);
        }
        else {
            if (flag == 1) {
                pk = ((ry * ry) * pow((x + 0.5),2)) + (pow(rx,2) * pow((y - 1),2)) - (pow(rx,2) * pow(ry,2));
                flag = 0;
            }
            if (pk > 0) {
                y--;
                pk += (rx * rx) - (2 * rx * rx * y);
            }
            else {
                x++;
                y--;
                pk += (ry * ry * 2 * x) + (rx * rx) - (2 * rx * rx * y);
            }
        }
        plotPoints(x1,y1,x, y);
        plotPoints(x1,y1,x, -y);
        plotPoints(x1,y1,-x, y);
        plotPoints(x1,y1,-x, -y);
    }
}

void plotMousePoints(int x, int y){
     glColor3f(0.0,0.0,0.0);
     glPointSize(5);
     glBegin(GL_POINTS);
     glVertex2f(x-250,250-y);
     glEnd();
     glFlush();
}
void mouseHandler(int button,int state,int mousex,int mousey){
     static int x1,y1,x2,y2,x3,y3;
     if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
     {
         x1=mousex;
         y1=mousey;
         plotMousePoints(x1,y1);}
   else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
     {
         x2=mousex;
         y2=mousey;
         plotMousePoints(x2,y2);
     }
     else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
     {
         x3=mousex;
         y3=mousey;
         plotMousePoints(x3,y3);
         ellipse(x1,y1,x2,y2,x3,y3);
     }
}
int main(int argc, char** argv)
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
     init();
     glutDisplayFunc(displayAxis);
     glutMouseFunc(mouseHandler);
     glutMainLoop();
     return 0;
}