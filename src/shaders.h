#include "mesh.h"
#include <string>

/* setup and return shader program by loading given shader files
 */
int setupBasicShader(std::string vertfile, std::string fragfile);

/* setup and return shader program with textures and 
 * blinn phong lighting model.  Sets textureID, and uses the
 * mesh's uv coordinates
 */
int setupTextureShader(GLuint &textureID, const mesh& object);
