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
#include <string.h>

#include "shaders.h"
#include "stb_image.h"

/* compile shaders and return their ids in a 2-vector */
glm::ivec2 loadShaders(std::string vertFile, std::string fragFile)
{

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* read shader files into strings */
    std::ifstream vfile(vertFile.c_str());
    std::ifstream ffile(fragFile.c_str());

    std::ostringstream vbuffer, fbuffer;
    vbuffer << vfile.rdbuf();
    fbuffer << ffile.rdbuf();
    
    std::string vertexString(vbuffer.str());
    std::string fragmentString(fbuffer.str());

    /* convert strings to c_strings and set as shader source */
    char *vertexCString, *fragmentCString;
    vertexCString = new char[vertexString.length() + 1];
    fragmentCString = new char[fragmentString.length() + 1];
    strcpy(vertexCString, vertexString.c_str());
    strcpy(fragmentCString, fragmentString.c_str());

    glShaderSource(vertexShader, 1, (const char**)&vertexCString, 0);
    glShaderSource(fragmentShader, 1, (const char**)&fragmentCString, 0);

    delete vertexCString;
    delete fragmentCString;

    /* compile shaders and check for errors */
    GLint status;
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        char log[2048];
        int len;
        glGetShaderInfoLog(vertexShader, 2048, (GLsizei*)&len, log);
        fprintf(stderr, "Vertex Shader: %s", log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        exit(1);
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        char log[2048];
        int len;
        glGetShaderInfoLog(fragmentShader, 2048, (GLsizei*)&len, log);
        fprintf(stderr, "Fragment Shader: %s", log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        exit(1);
    }

    return glm::ivec2(vertexShader, fragmentShader);
}

int setupBasicShader(std::string vertfile, std::string fragfile)
{
    /* init glew and load compiled shaders */
    glewInit();
    if(!glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object")) 
    {
        fprintf(stderr, "Required OpenGL extensions missing\n");
        return -1;
    }
    glm::ivec2 shaders = loadShaders(vertfile, fragfile);
    GLuint vertexShader = shaders.x;
    GLuint fragmentShader = shaders.y;

    /* create and link program */
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

int setupTextureShader(GLuint &textureID, const mesh& object)
{
    glewInit();
    if(!glewIsSupported("GL_VERSION_2_0 GL_ARB_multitexture GL_EXT_framebuffer_object")) 
    {
        fprintf(stderr, "Required OpenGL extensions missing\n");
        return -1;
    }

    /* create texture */
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* load texture file */
    int width, height, components;
    unsigned char *data = stbi_load("static/uv_color_map.png",
            &width, &height, &components, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);

    /* load UV data from mesh */
    std::vector<glm::vec2> texture_uvs = object.getTextureUVs();
    unsigned int uvsize = texture_uvs.size();
    unsigned int uvmem = 2*uvsize*sizeof(GLfloat);
    GLfloat *uv_buffer_data = (GLfloat *)malloc(uvmem);
    for(int i = 0; i < uvsize; i++)
    {
        uv_buffer_data[2*i] = texture_uvs.at(i).x;
        uv_buffer_data[2*i + 1] = texture_uvs.at(i).y;
    }

    /* pass data to shader */
    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvmem, uv_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    free(uv_buffer_data);

    glm::ivec2 shaders = loadShaders("src/texture.vert", "src/texture.frag");
    GLuint vertexShader = shaders.x;
    GLuint fragmentShader = shaders.y;

    /* create and link program */
    int program = glCreateProgram();

    glBindAttribLocation(program, 0, "vertexUV");
    int loc = glGetUniformLocation(program, "texsampler");
    glUniform1i(loc, 0);

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
