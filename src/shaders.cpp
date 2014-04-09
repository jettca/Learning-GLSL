#include <GL/glew.h>
#if defined(_WIN32)
#   include <GL/wglew.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>

#include "shaders.h"
#include "stb_image.h"

int loadShaders(std::string vertFile, std::string fragFile)
{
    glewInit();
    if(!glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object")) 
    {
        fprintf(stderr, "Required OpenGL extensions missing\n");
        return -1;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* read in shader files */
    std::ifstream v(vertFile);
    std::ifstream f(fragFile);

    std::ostringstream vbuffer, fbuffer;
    vbuffer << v.rdbuf();
    fbuffer << f.rdbuf();

    std::string s_phongVS(vbuffer.str());
    std::string s_phongFS(fbuffer.str());

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
        return -1;
    }

    int program = glCreateProgram();
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
        return -1;
    }

    return program;
}

int setupPhongShader()
{
    return loadShaders("src/phong.vert", "src/phong.frag");
}

int setupTextureShader(GLuint &textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, components;
    unsigned char *data = stbi_load("static/uv_color_map.png",
            &width, &height, &components, 0);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);

    return loadShaders("src/phong.vert", "src/phong.frag");
}
