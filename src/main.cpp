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

using namespace std;

#define BUFFER_LENGTH 64

GLfloat camPosX, camPosY, camPosZ;

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

// Shaders
GLuint program;

void initLights()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void setupRC()
{
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = -10.5f;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    initLights();
}

void setCamera()
{
    glTranslatef(camPosX, camPosY, camPosZ);
}

void drawTeapot()
{
    glUseProgram(program);

    GLint loc = glGetUniformLocation(program, "lightDir");
    glUniform3f(loc, 2, -1, .3);

    GLfloat selectedColor[] = {0, 1, 0, 1};
    GLfloat unselectedColor[] = {1, 0, 0, 1};
    
    glPushMatrix();
    {
        glutSolidTeapot(2.5);
    }
    glPopMatrix();

    glUseProgram(0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    {
        setCamera();
        drawTeapot();
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

static void setupShaders()
{
    printf("Initializing GLSL Shaders...\n");

    glewInit();
    if(!glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object")) 
    {
        fprintf(stderr, "Required OpenGL extensions missing\n");
        exit(2);
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* read in shader files */
    ifstream v("src/toon.vert");
    ifstream f("src/toon.frag");

    ostringstream vbuffer, fbuffer;
    vbuffer << v.rdbuf();
    fbuffer << f.rdbuf();

    string s_toonVS(vbuffer.str());
    string s_toonFS(fbuffer.str());

    char *toonVS, *toonFS;
    toonVS = new char[s_toonVS.length() + 1];
    toonFS = new char[s_toonFS.length() + 1];
    strcpy(toonVS, s_toonVS.c_str());
    strcpy(toonFS, s_toonFS.c_str());

    glShaderSource(vertexShader, 1, (const char**)&toonVS, 0);
    glShaderSource(fragmentShader, 1, (const char**)&toonFS, 0);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
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

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    glDeleteProgram(program);
}
