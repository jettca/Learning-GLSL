Learning-GLSL
=================
A few shaders.

Contents
I.      Directions
II.     Features
III.    Implementation
IV.     Sources


I. Directions
Execute "cmake .; make" to compile the code.
Run it with "./pa3 N" where N selects a shader and can be 1, 2, or 3.
    1: Gourand shader
    2: Blinn-Phong shader
    3: Color texture + Blinn-Phong shader
Use the left and right arrow keys to move the model.


II. Features
For this project, I wrote a .obj file loader and a mesh class in which to load
them.  It also loads the material and uv coordinates contained in the
associated .mtl file.  I use this to load a sideways-spaceship-looking model I created in
Maya.

The Gourand shader and Blinn-Phong shader calculate the
normal, viewing, and light directions per vertex, and then they are interpolated
and used to calculate color.  This differs from the default OpenGL
implementation, where colors are calculated at the vertices and interpolated
from there.  The Gourand shader is glossy and somewhat unrealistic seeming,
compared to the smoother Blinn-Phong shader.

The texture shader doesn't actually work.  I came as close as I could, and was
left with some very strange artifacts.  The texture is loaded and set, but
somehow, when it gets sampled, there is a lot of noise and strange artifacts,
almost like the texture were being randomly sampled rather than evenly laid out
over the surface.  One of the challenges in making this work is my graphics
card's lack of support for OpenGL 3, which features the now most commonly used
methods for texturing and GLSL in general.  However, with some texture files,
the results are still very interesting looking.


III. Implementation
The implementation was fairly straightforward for the first two shaders.  I used
STL as much as possible for the .obj file loader, and learned more about that in
the process.  For texturing, the implementation challenges were almost entirely
related to trying to understand the OpenGL texturing interface.


IV. Sources
Until I realized I couldn't actually use many modern OpenGL features, I read a
lot in opengl-tutorial.org.  Sadly, I forgot about the provided legacy OpenGL
tutorial provided in the programming assignment until too late, though it's not
clear whether or not it would have fixed my issues.
