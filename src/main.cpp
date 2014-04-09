#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#if defined(_WIN32)
#   include <GL/wglew.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "mesh.h"

using namespace std;

#define BUFFER_LENGTH 64

GLfloat camPosX, camPosY, camPosZ, camRotY;

// Lights & Materials
GLfloat position[] = {3.0, 2.0, 2.0, 0.0};

// Shaders
GLuint program;

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
    camPosZ = -10.5f;
    camRotY = 0.0f;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();
}

void loadThing(string filepath)
{
    thing = mesh(filepath);
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

static void setupShaders()
{
    glewInit();
    if(!glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object")) 
    {
        fprintf(stderr, "Required OpenGL extensions missing\n");
        exit(2);
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* read in shader files */
    ifstream v("src/phong.vert");
    ifstream f("src/phong.frag");

    ostringstream vbuffer, fbuffer;
    vbuffer << v.rdbuf();
    fbuffer << f.rdbuf();

    string s_phongVS(vbuffer.str());
    string s_phongFS(fbuffer.str());

    char *phongVS, *phongFS;
    phongVS = new char[s_phongVS.length() + 1];
    phongFS = new char[s_phongFS.length() + 1];
    strcpy(phongVS, s_phongVS.c_str());
    strcpy(phongFS, s_phongFS.c_str());

    glShaderSource(vertexShader, 1, (const char**)&phongVS, 0);
    glShaderSource(fragmentShader, 1, (const char**)&phongFS, 0);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLint vstatus, fstatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vstatus);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fstatus);
    if(vstatus != GL_TRUE || fstatus != GL_TRUE)
    {
        char log[2048];
        int len;
        glGetShaderInfoLog(vertexShader, 2048, (GLsizei*)&len, log);
        fprintf(stderr, "%s", log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        exit(1);
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint pstatus;
    glGetProgramiv(program, GL_LINK_STATUS, &pstatus);
    if(pstatus != GL_TRUE)
    {
        char log[2048];
        int len;
        glGetProgramInfoLog(program, 2048, (GLsizei*)&len, log);
        fprintf(stderr, "%s", log);
        glDeleteProgram(program);
        exit(1);
    }
}

int main(int argc, char **argv)
{
    int win_width = 1024;
    int win_height = 512;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);

    glutCreateWindow("wow");
    setupShaders();
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
