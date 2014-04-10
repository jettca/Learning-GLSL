#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#if defined(_WIN32)
#   include <GL/wglew.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "shaders.h"
#include "mesh.h"

using namespace std;

#define BUFFER_LENGTH 64

GLfloat camPosX, camPosY, camPosZ, camRotY;

// Lights & Materials
GLfloat position[] = {2.5, 1.0, 2.0, 0.0};

// Shaders and textures
int shaderMode;
int program;
GLuint textureID;

// Mesh data
mesh thing;

// Input
bool UDRL[4] = {false, false, false, false};

void initLights()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void setupRC()
{
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -15.0f;
    camRotY = 0.0f;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();
}

void loadThing(string filepath)
{
    /* load mesh */
    thing = mesh(filepath);

    /* load shader */
    if(shaderMode == 1)
    {
        program = setupBasicShader("src/phong.vert", "src/phong.frag");
    }
    else if(shaderMode == 2)
    {
        program = setupBasicShader("src/bp.vert", "src/bp.frag");
    }
    else
    {
        program = setupTextureShader(textureID, thing);
    }
    if(program < 0)
        exit(1);
}

void renderThing()
{
    glUseProgram(program);

    glPushMatrix();
    {
        glRotatef(camRotY, 0, 1, 0);
        thing.render();
    }
    glPopMatrix();

    glUseProgram(0);
}

void setCamera()
{
    glTranslatef(camPosX, camPosY, camPosZ);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    {
        setCamera();
        renderThing();
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: // Escape key
            exit(0);
            break;
    }
}

void special(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_RIGHT:
            UDRL[2] = true;
            break;
        case GLUT_KEY_LEFT:
            UDRL[3] = true;
            break;
    }
}

void specialUp(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_RIGHT:
            UDRL[2] = false;
            break;
        case GLUT_KEY_LEFT:
            UDRL[3] = false;
            break;
    }
}

void update()
{
    if(UDRL[2])
    {
        camRotY += 2;
    }
    if(UDRL[3])
    {
        camRotY -= 2;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    int win_width = 1024;
    int win_height = 512;

    if(argc != 2)
    {
        cout << "One argument required\n";
        exit(1);
    }
    shaderMode = atoi(argv[1]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);

    glutCreateWindow("wow");

    setupRC();
    loadThing("static/test_mesh.obj");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(update);

    glutMainLoop();

    glDeleteProgram(program);
}
