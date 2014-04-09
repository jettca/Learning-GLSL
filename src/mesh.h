#include <string>
#include <vector>
#include "glm/glm.hpp"

class mesh
{
    public:
        mesh();
        mesh(std::string filepath);
        void render();

    private:
        /* faces is one row per vertex, one column per data point
         * data points are corresponding indices for:
         * vertices, uv_textures, and normals in that order
         */
        std::vector<glm::mat3> faces;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uv_textures;
        std::vector<glm::vec3> normals;

        glm::vec3 diffuse, ambient;

        void loadMtl(std::string mtlpath);
};
