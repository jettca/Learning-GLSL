#include <iostream>
#include <fstream>
#include <sstream>

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "mesh.h"

mesh::mesh() :
    vertices(),
    uv_textures(),
    normals(),
    diffuse(),
    ambient()
{}

mesh::mesh(std::string filepath) :
    vertices(),
    uv_textures(),
    normals(),
    diffuse(),
    ambient()
{
    std::ostringstream mtlpath;
    mtlpath << "static/";

    std::ifstream objfile(filepath);
    std::string line;
    while(getline(objfile, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if(type == "v")
        {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(glm::vec3(x, y, z));
        }
        else if(type == "vt")
        {
            float u, v;
            iss >> u >> v;
            uv_textures.push_back(glm::vec2(u, v));
        }
        else if(type == "vn")
        {
            float x, y, z;
            iss >> x >> y >> z;
            normals.push_back(glm::vec3(x, y, z));
        }
        else if(type == "f")
        {
            glm::mat3 face;
            int v, vt, vn;
            char slash;
            for(int i = 0; i < 3; i++)
            {
                iss >> v >> slash >> vt >> slash >> vn;
                face[i] = glm::vec3(v, vt, vn);
            }
            faces.push_back(face);
        }
        else if(type == "mtllib")
        {
            std::string temp;
            iss >> temp;
            mtlpath << temp;
        }
    }

    loadMTL(mtlpath.str());
}

void mesh::loadMTL(std::string mtlpath)
{
    std::ifstream mtlfile(mtlpath);
    std::string line;
    while(getline(mtlfile, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if(type == "Kd")
        {
            iss >> diffuse.x >> diffuse.y >> diffuse.z;
        }
        else if(type == "Ka")
        {
            iss >> ambient.x >> ambient.y >> ambient.z;
        }
    }
}

void mesh::render()
{
    GLfloat mat_diffuse[4] = {diffuse.x, diffuse.y, diffuse.z, 1.0f};
    GLfloat mat_ambient[4] = {ambient.x, ambient.y, ambient.z, 1.0f};
    GLfloat mat_specular[4] = {diffuse.x, diffuse.y, diffuse.z, 1.0f};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

    glBegin(GL_TRIANGLES);
    unsigned int numfaces = faces.size();
    for(int i = 0; i < numfaces; i++)
    {
        glm::mat3 face = faces.at(i);
        for(int v = 0; v < 3; v++)
        {
            glm::vec3 vertex = vertices.at(face[v][0]-1);
            glm::vec2 uv_texture = uv_textures.at(face[v][1]-1);
            glm::vec3 normal = normals.at(face[v][2]-1);

            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();
}

std::vector<glm::vec2> mesh::getTextureUVs() const
{
    return uv_textures;
}
